/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_UTILITIES_HPP
#define COIN_UTILITIES_HPP

#include <common/Base58CheckEncodable.hpp> // version macroes
#include <common/AddressType.hpp>
#include <stdutils/uchar_vector.h>
#include <string>

#define DEFAULT_SEQUENCE_NUMBER 0xFFFFFFFF

namespace Coin {

    std::string toHex(const std::vector<unsigned char> &input);
    std::string toHex(const unsigned char * input, size_t length);
    std::vector<unsigned char> fromHex(const std::string& input);

    // Used to provide version array for mSIGNA libs.
    // Returns 1 byte version byte for p2pkh and p2sh addresses on given network
    enum class Network;

    const unsigned char testnet3AddressVersions[] = {P2PKH_ADDRESS_TESTNET3_VERSION_VALUE, P2SH_ADDRESS_TESTNET3_VERSION_VALUE};
    const unsigned char mainnetAddressVersions[] = {P2PKH_ADDRESS_MAINNET_VERSION_VALUE, P2SH_ADDRESS_MAINNET_VERSION_VALUE};

    const unsigned char * networkToAddressVersions(Network network);

    /**
     * Routines for generating version bytes for bip32 extended key serialization
     * 4 byte: version bytes (mainnet: 0x0488B21E public, 0x0488ADE4 private; testnet: 0x043587CF public, 0x04358394 private)
     */
    unsigned int extendedPrivateKeyVersionBytes(Network network);
    unsigned int extendedPublicKeyVersionBytes(Network network);

    /**
     * Process Bitcoin addresses according to
     * https://en.bitcoin.it/wiki/List_of_address_prefixes
     */

    // Generate version byte
    unsigned int toBase58CheckVersionBytes(AddressType type, Network network);

    // Recover address type from version byte
    AddressType versionByteToAddressType(unsigned int version);

    // Recover network type from version byte
    Network versionByteToNetwork(unsigned int version);

    // Raw script for pushing given number of bytes
    // Used for constructing raw scripts
    // Copied from mSIGNA
    uchar_vector opPushData(uint32_t nBytes);

    // used for pushing a number to the stack in scripts for operations that interpret the item on the stack
    // as a number
    uchar_vector opPushNumber(uint32_t value);

    uchar_vector popData(const uchar_vector & script, uchar_vector & poppedData);

    // Deduce address network
    //Network getNetwork(std::string & base58CheckEncodedAddress);

    // Deduce address type
    //AddressType getType(std::string & base58CheckEncodedAddress);

}


/**
 * Signing and spending stuff
 */

#include <CoinCore/CoinNodeData.h>
//#include <CoinQ/CoinQ_script.h> // Declare CoinQ::Script::Script::type_t, is internal to class, so cannot be forward declared

namespace Coin {

    //enum class SigHashType;
    class SigHashType;
    class typesafeOutPoint;

    /**
     * MOVE INTO SIGHASHTYPE.HPP FILE
     */

    // Signature hash of type <sighahType> for transaction <tx> corresponding to input
    // index <input> spending outpoint
    uchar_vector sighash(const Coin::Transaction & tx,
                    uint inputIndex,
                    const uchar_vector & subscript,
                    const SigHashType & sigHashType);

    // Signature hash of type <sighahType> for transaction <tx> corresponding to outpoint
    // <outPoint>
    uchar_vector sighash(const Coin::Transaction & tx,
                    const Coin::typesafeOutPoint & outPoint,
                    const uchar_vector & subscript,
                    const SigHashType & sigHashType);

    /**
    // DER encoded signature (without trailing sighash flag) corresponding to <privateKey> on input <inputToSign> of transaction <tx>.
    // All scriptSigs in <tx> are irrelevant to resulting signature, and might as well be empty
    secure_bytes_t createSignature(const Coin::Transaction & tx,
                                   uint inputToSign,
                                   const CoinQ::Script::Script & inputScriptBuilder,
                                   const uchar_vector & privateKey);

    // Whether DER encoded signature <signature> (without trailing sighash flag) is a valid SIGHASH_ALL signature for public key <publicKey> on input <inputToCheck> of transaction <tx>
    // All scriptSigs in <tx> are irrelevant to resulting signature, and might as well be empty
    bool verifySignature(const Coin::Transaction & tx,
                         uint inputToCheck,
                         const CoinQ::Script::Script & inputScriptBuilder,
                         const secure_bytes_t & signature,
                         const bytes_t & publicKey);

    */

    // Turns into raw: OP_0 ...signatures...
    // which is used for both p2sh and regular mofn scriptSig
    class TransactionSignature;
    uchar_vector serializeForOP_CHECKSIGMULTISIG(const std::vector<TransactionSignature> & sigs);

    class PrivateKey;

    uchar_vector serializeScriptNum(const int64_t& value);
    int64_t deserializeScriptNum(const uchar_vector & vch);
}
#endif // COIN_UTILITIES_HPP
