/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 7 2015
 */

#include <common/SigHashType.hpp>
#include <common/Utilities.hpp>
#include <CoinCore/CoinNodeData.h> //Coin::HashType
#include <CoinCore/numericdata.h>

#include <stdexcept>
#include <cassert>

// SIGHASHTYPE_BITMASK defines the number of bits of the hash type which is used
// to identify which outputs are signed.
#define SIGHASHTYPE_BITMASK 0x1f

#define SIGHASH_SINGLE_BUG "0000000000000000000000000000000000000000000000000000000000000001"

namespace Coin {

// Not including a default constructor to make sure network is always explicitly set
// SigHashType::SigHashType()
//     : _type(MutuallyExclusiveType::all)
//     , _anyOneCanPay(false)
//     , _chain(ChainType::bitcoin) {
// }

SigHashType::SigHashType(MutuallyExclusiveType type, bool anyOneCanPay, Network network)
    : _type(type)
    , _anyOneCanPay(anyOneCanPay)
    , _chain(chainTypeFromNetwork(network)) {
}

SigHashType::SigHashType(MutuallyExclusiveType type, bool anyOneCanPay, ChainType chainType)
    : _type(type)
    , _anyOneCanPay(anyOneCanPay)
    , _chain(chainType) {
}

bool SigHashType::operator==(const SigHashType & rhs) {
    return _type == rhs.type() && _anyOneCanPay == rhs.anyOneCanPay() && _chain == rhs.chainType();
}

SigHashType SigHashType::fromHashCode(uint32_t hashCode) {

    uint32_t masked = (hashCode & SIGHASHTYPE_BITMASK);

    MutuallyExclusiveType type;

    if(flag(MutuallyExclusiveType::all) == masked)
        type = MutuallyExclusiveType::all;
    else if(flag(MutuallyExclusiveType::none) == masked)
        type = MutuallyExclusiveType::none;
    else if(flag(MutuallyExclusiveType::single) == masked)
        type = MutuallyExclusiveType::single;
    else
        throw std::runtime_error("No standard mandatory sighash flag was set.");


    return SigHashType(type, canAnyonePay(hashCode), chainTypeFromHashCode(hashCode));
}

bool SigHashType::canAnyonePay(uint32_t hashCode) {
    return hashCode & HashType::SIGHASH_ANYONECANPAY;
}

SigHashType SigHashType::standard(Network network) {
    return SigHashType(MutuallyExclusiveType::all, false, network);
}

bool SigHashType::isStandard() const {
    return (_type == MutuallyExclusiveType::all) && !_anyOneCanPay;
}

unsigned char SigHashType::flag(MutuallyExclusiveType type) {

    switch(type) {

        case MutuallyExclusiveType::all: return HashType::SIGHASH_ALL;
        case MutuallyExclusiveType::none: return HashType::SIGHASH_NONE;
        case MutuallyExclusiveType::single: return HashType::SIGHASH_SINGLE;

        default:
            throw std::runtime_error("Coding error, unsupported MutuallyExclusiveType");
    }
}

uint32_t SigHashType::hashCode() const {
    uint32_t hashCode = flag(_type) | ( _anyOneCanPay ? HashType::SIGHASH_ANYONECANPAY : 0);

    // Bitcoin
    if (ChainType::bitcoin == _chain) {
      return hashCode;
    }

    // Fork
    hashCode |= HashType::SIGHASH_FORKID;

    // Bitcoin Cash
    if (ChainType::cash == _chain) {
      hashCode |= (BITCOIN_CASH_FORK_ID << 8);
      return hashCode;
    }

    throw std::runtime_error("unsupported chain fork");
}

uchar_vector SigHashType::hashCodeForScript() const {
  uchar_vector codeForScript;

  unsigned char code = hashCode() & 0x000000ff;
  codeForScript.push_back(code);
  return codeForScript;
}

SigHashType::MutuallyExclusiveType SigHashType::type() const {
    return _type;
}

void SigHashType::setType(MutuallyExclusiveType type) {
    _type = type;
}

bool SigHashType::anyOneCanPay() const {
    return _anyOneCanPay;
}

void SigHashType::setAnyOneCanPay(bool anyOneCanPay) {
    _anyOneCanPay = anyOneCanPay;
}

void SigHashType::setNetwork (Network network) {
  _chain = chainTypeFromNetwork(network);
}

SigHashType::ChainType SigHashType::chainTypeFromHashCode(uint32_t hashCode) {
  if (hashCode & HashType::SIGHASH_FORKID) {
    // 24 most significant bits of hashcode stores the fork id value
    auto forkid = hashCode >> 8;

    if (BITCOIN_CASH_FORK_ID == forkid) {
      // Bitcoin Cash
      return SigHashType::ChainType::cash;
    }

    throw std::runtime_error("Unknown fork id in sighash code");

  } else {
    return ChainType::bitcoin;
  }
}

SigHashType::ChainType SigHashType::chainTypeFromNetwork (Network network) {
  // Bitcoin networks
  if (Network::mainnet == network || Network::testnet3 == network || Network::regtest == network) {
    return ChainType::bitcoin;
  }

  // Bitcoin Cash networks
  if (Network::mainnet_bitcoin_cash == network || Network::testnet_bitcoin_cash == network || Network::regtest_bitcoin_cash == network) {
    return ChainType::cash;
  }

  throw std::runtime_error("unsupported network");
}

SigHashType::ChainType SigHashType::chainType() const {
  return _chain;
}

uchar_vector SigHashType::getSigHash(const Transaction &tx,
                     uint inputIndex,
                     const uchar_vector &subscript,
                     uint64_t value) const {
    // Bitcoin
    if (ChainType::bitcoin == _chain) {
      if (!tx.inputs[inputIndex].scriptWitness.isEmpty()) {
        // Bitcoin segwit implemented in CoinCore
        return tx.getSigHash(hashCode(), inputIndex, subscript, value);
      } else {
        // Bitcoin non-segwit
        return sighash_bitcoin_old(tx, inputIndex, subscript, hashCode());
      }
    }

    // Bitcoin Cash
    if (ChainType::cash == _chain) {
      return sighash_bitcoin_cash(tx, inputIndex, subscript, hashCode(), value);
    }

    throw std::runtime_error("sighash: unsupported chain type");
}

uchar_vector SigHashType::getSigHash(const Coin::Transaction & tx,
                const typesafeOutPoint &outPoint,
                const uchar_vector & subscript,
                uint64_t value) const {

    // find input index by outpoint
    for(uint n = 0; n < tx.inputs.size(); n++) {
        if(typesafeOutPoint(tx.inputs[n].previousOut) == outPoint) {
            return getSigHash(tx, n, subscript, value);
        }
    }

    throw std::runtime_error("sighash: transaction does not have a corresponding input");
}

// Old Sighash for bitcoin non-segwit transactions
uchar_vector sighash_bitcoin_old(const Transaction &tx,
                     uint inputIndex,
                     const uchar_vector &subscript,
                     uint32_t hashcode) {

    if (hashcode & HashType::SIGHASH_FORKID)
      throw std::runtime_error("cannot set forkid flag for bitcoin sighash");

    // Should not be calling this function for segwit transactions!
    if(!tx.inputs[inputIndex].scriptWitness.isEmpty())
      throw std::runtime_error("this sighash cannot be used for segwit transactions");

    if(inputIndex >= tx.inputs.size()) {
        throw std::runtime_error("sighash: input index out of range");
    }

    // Make a copy of the transaction
    Coin::Transaction txCopy = tx;

    // Clear all inputs scripts
    txCopy.clearScriptSigs();

    //TODO: must remove code separators:
    // uchar_vector subscript_cleared = removeCodeSeparators(subscript);
    uchar_vector subscript_cleared = subscript;

    // Set script sig to subscript for signature (must have code separators removed)
    txCopy.inputs[inputIndex].scriptSig = subscript_cleared;

    if((hashcode & SIGHASHTYPE_BITMASK) == HashType::SIGHASH_SINGLE ||
       (hashcode & SIGHASHTYPE_BITMASK) == HashType::SIGHASH_NONE) {
        // clear sequence numbers
        for (int i = 0; i < txCopy.inputs.size(); i++) {
          if (inputIndex != i) {
            txCopy.inputs[i].sequence = 0;
          }
        }
    }

    if((hashcode & SIGHASHTYPE_BITMASK) == HashType::SIGHASH_NONE) {
      // Remove all outputs
      txCopy.outputs.clear();

    } else if ((hashcode & SIGHASHTYPE_BITMASK) == HashType::SIGHASH_SINGLE) {
      // The SIGHASH_SINGLE bug.
      // https://bitcointalk.org/index.php?topic=260595.0
      if (inputIndex >= txCopy.outputs.size()) {
        return uchar_vector(SIGHASH_SINGLE_BUG);
      }

      txCopy.outputs.resize(inputIndex + 1);

      for (int i = 0; i < inputIndex; i++) {
        txCopy.outputs[i].value = 0xffffffffffffffff; // (long) -1
        txCopy.outputs[i].scriptPubKey = uchar_vector(); // empty script
      }
    }

    if(hashcode & HashType::SIGHASH_ANYONECANPAY) {
        // Sign only one input
        // https://en.bitcoin.it/wiki/OP_CHECKSIG#Procedure_for_Hashtype_SIGHASH_ANYONECANPAY

        // Remove all inputs
        txCopy.inputs.clear();

        // Add input to sign as input 0
        txCopy.inputs.push_back(tx.inputs[inputIndex]);

        // Set script sig to subscript for signature
        txCopy.inputs[0].scriptSig = subscript_cleared;
    }

    // Compute sighash
    return txCopy.getHashWithAppendedCode(hashcode);
}

// Bitcoin Cash sighash algorithm
uchar_vector sighash_bitcoin_cash(const Transaction &tx,
                     uint inputIndex,
                     const uchar_vector &subscript,
                     uint32_t hashcode,
                     uint64_t value) {

     if (!(hashcode & HashType::SIGHASH_FORKID))
       throw std::runtime_error("forkid flag not set for bitcoin cash sighash");

     // Should not be calling this function for segwit transactions!
     if(!tx.inputs[inputIndex].scriptWitness.isEmpty())
       throw std::runtime_error("this sighash cannot be used for segwit transactions");

     if(inputIndex >= tx.inputs.size()) {
         throw std::runtime_error("sighash: input index out of range");
     }

     uchar_vector hashPrevouts;
     uchar_vector hashSequence;
     uchar_vector hashOutputs;

     if(!(hashcode & HashType::SIGHASH_ANYONECANPAY)) {
         uchar_vector ss;
         for (auto& input: tx.inputs) { ss += input.previousOut.getSerialized(); }
         hashPrevouts = sha256_2(ss);
     } else {
         hashPrevouts = uchar_vector(32); //uint256 of 0x0000......0000
     }

     if(!(hashcode & HashType::SIGHASH_ANYONECANPAY) &&
        (hashcode & SIGHASHTYPE_BITMASK) != HashType::SIGHASH_SINGLE &&
        (hashcode & SIGHASHTYPE_BITMASK) != HashType::SIGHASH_NONE) {
         uchar_vector ss;
         for (auto& input: tx.inputs) { ss += uint_to_vch(input.sequence, LITTLE_ENDIAN_); }
         hashSequence = sha256_2(ss);
     } else {
         hashSequence = uchar_vector(32); //uint256 of 0x0000......0000
     }

     if((hashcode & SIGHASHTYPE_BITMASK) != HashType::SIGHASH_SINGLE &&
        (hashcode & SIGHASHTYPE_BITMASK) != HashType::SIGHASH_NONE) {
         uchar_vector ss;
         for (auto& output: tx.outputs) { ss += output.getSerialized(); }
         hashOutputs = sha256_2(ss);
     } else if((hashcode & SIGHASHTYPE_BITMASK) == HashType::SIGHASH_SINGLE &&
                inputIndex < tx.outputs.size()){
        uchar_vector ss;
        ss += tx.outputs[inputIndex].getSerialized();
        hashOutputs = sha256_2(ss);
     } else {
        hashOutputs = uchar_vector(32); //uint256 of 0x0000......0000
     }

     uchar_vector ss;
     ss += uint_to_vch(tx.version, LITTLE_ENDIAN_);
     ss += hashPrevouts;
     ss += hashSequence;
     ss += tx.inputs[inputIndex].previousOut.getSerialized();
     ss += VarInt(subscript.size()).getSerialized();
     ss += subscript;
     ss += uint_to_vch(value, LITTLE_ENDIAN_);
     ss += uint_to_vch(tx.inputs[inputIndex].sequence, LITTLE_ENDIAN_);
     ss += hashOutputs;
     ss += uint_to_vch(tx.lockTime, LITTLE_ENDIAN_);
     ss += uint_to_vch(hashcode, LITTLE_ENDIAN_);

     return sha256_2(ss);
}

}
