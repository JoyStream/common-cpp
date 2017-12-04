/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#include <stdint.h>
#include <common/Network.hpp>
#include <common/typesafeOutPoint.hpp>
#include <CoinCore/CoinNodeData.h>

#ifndef COMMON_SIGHASHTYPE_HPP
#define COMMON_SIGHASHTYPE_HPP

#define BITCOIN_CASH_FORK_ID 0

namespace Coin {

class SigHashType {

public:

    // You can only be have one of these
    enum class MutuallyExclusiveType {
        all,
        none,
        single
    };

    //SigHashType();

    SigHashType(MutuallyExclusiveType type, bool anyOneCanPay, Network network);

    bool operator==(const SigHashType & rhs);

    // From (little endian) hash code, as it appears in sighash computation step
    static SigHashType fromHashCode(uint32_t hashCode);

    // Whether (little endian) hash code has anyonecan pay flag set
    static bool canAnyonePay(uint32_t hashCode);

    // Returns type all and !anyonecanpay
    static SigHashType standard(Network network);

    // Is of type all and !anyonecanpay
    bool isStandard() const;

    // Returns zeroed out byte with flag set for *only* given hash type
    static unsigned char flag(MutuallyExclusiveType type);

    // To (little endian) hash code, as it appears in sighash computation step
    uint32_t hashCode() const;

    // hash code byte array used in a transaction signature
    uchar_vector hashCodeForScript() const;

    // Signature hash for transaction <tx> corresponding to input index <input>
    uchar_vector getSigHash(const Coin::Transaction & tx,
                    uint inputIndex,
                    const uchar_vector & subscript,
                    uint64_t value = 0) const;

    // Signature hash for transaction <tx> corresponding to outpoint <outPoint>
    uchar_vector getSigHash(const Coin::Transaction & tx,
                    const Coin::typesafeOutPoint & outPoint,
                    const uchar_vector & subscript,
                    uint64_t value = 0) const;

    // Getters and setters
    MutuallyExclusiveType type() const;
    void setType(MutuallyExclusiveType type);

    bool anyOneCanPay() const;
    void setAnyOneCanPay(bool anyOneCanPay);

    // Sets the chain type based on the Network
    void setNetwork(Network network);

private:

    // The chain for which a sighash will be generated for
    enum class ChainType {
      bitcoin, // Bitcoin, Litecoin
      cash // Bitcoin Cash
    };

    SigHashType(MutuallyExclusiveType type, bool anyOneCanPay, ChainType chainType);

    static ChainType chainTypeFromHashCode(uint32_t hashCode);
    static ChainType chainTypeFromNetwork(Network network);

    MutuallyExclusiveType _type;
    ChainType _chain;
    bool _anyOneCanPay;

    ChainType chainType() const;

    // Old (non-segwit) Bitcoin sighash
    uchar_vector sighash_bitcoin_old(const Coin::Transaction & tx,
                    uint inputIndex,
                    const uchar_vector & subscript) const;

    // Bitcoin Cash sighash algorithm - revised BIP143 to account for non-Segwit deployment.
    // see https://github.com/Bitcoin-UAHF/spec/blob/master/replay-protected-sighash.md for details
    uchar_vector sighash_bitcoin_cash(const Coin::Transaction & tx,
                    uint inputIndex,
                    const uchar_vector & subscript,
                    uint64_t value) const;
};

}

#endif // COMMON_SIGHASHTYPE_HPP
