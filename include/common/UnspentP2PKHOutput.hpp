/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */
/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#ifndef COIN_UNSPENT_P2PKH_OUTPUT_HPP
#define COIN_UNSPENT_P2PKH_OUTPUT_HPP

#include <common/P2PKHScriptPubKey.hpp>
#include <common/UnspentOutput.hpp>
#include <common/KeyPair.hpp>
#include <common/TransactionSignature.hpp>

namespace Coin {

class UnspentP2PKHOutput : public UnspentOutput
{
public:

    // Default constructor
    UnspentP2PKHOutput();

    // Constructor from members
    UnspentP2PKHOutput(const KeyPair & keyPair, const typesafeOutPoint & outPoint, uint64_t setValue);

    // Comparator
    bool operator==(const UnspentP2PKHOutput & o) const;
    bool operator!=(const UnspentP2PKHOutput & o) const;

    // Getters and setters
    KeyPair keyPair() const;
    void setKeyPair(const KeyPair & keyPair);

    virtual uchar_vector scriptPubKey() const;
    virtual uchar_vector sighash(const Transaction & tx, const SigHashType &sigHashType) const;
    virtual TransactionSignature transactionSignature(const Transaction & tx, const SigHashType & sigHashType) const;
    virtual uchar_vector scriptSig(const Transaction &, const SigHashType &) const;

private:

    // Controls utxo
    KeyPair _keyPair;
};

}

#endif // COIN_UNSPENT_P2PKH_OUTPUT_HPP
