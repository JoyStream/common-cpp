/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_UNSPENT_OUTPUT_HPP
#define COIN_UNSPENT_OUTPUT_HPP

#include <common/typesafeOutPoint.hpp>
#include <common/SigHashType.hpp>

#include <CoinCore/typedefs.h>

namespace Coin {

class TransactionSignature;
class TxIn;

class UnspentOutput {
public:

    // Default constructor
    UnspentOutput();

    // Constructor from members
    UnspentOutput(const typesafeOutPoint & outPoint, uint64_t setValue);

    // Comparator
    bool operator==(const UnspentOutput & o) const;
    bool operator!=(const UnspentOutput & o) const;

    // Getters and setters
    typesafeOutPoint outPoint() const;
    void setOutPoint(const typesafeOutPoint & outPoint);

    uint64_t value() const;
    void setValue(uint64_t value);

    virtual uchar_vector scriptPubKey() const = 0;
    virtual uchar_vector sighash(const Transaction & tx, const SigHashType &sigHashType) const = 0;
    virtual TransactionSignature transactionSignature(const Transaction & tx, const SigHashType & sigHashType) const = 0;
    virtual uchar_vector scriptSig(const Transaction & tx, const SigHashType & sigHashType) const = 0;

    // nSequence value to be used for the input in the spending transaction
    // If not overriden in dervied classes it just returns the default sequence number which disabled nLockTime
    virtual uint32_t spendingInputSequenceNumber() const;

    // Returns a Coin::TxIn with the correct outpoint and sequence number, without a scriptSig (input script) set
    Coin::TxIn unsignedSpendingInput() const;

private:

    // OutPoint locating utxo
    typesafeOutPoint _outPoint;

    // Value of output
    uint64_t _value;
};

}

#endif // COIN_UNSPENT_OUTPUT_HPP
