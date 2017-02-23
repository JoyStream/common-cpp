/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#ifndef COIN_PAYMENT_HPP
#define COIN_PAYMENT_HPP

#include <common/P2PKHAddress.hpp>
#include <common/P2SHAddress.hpp>
#include <common/RedeemScriptHash.hpp>

namespace Coin {

    class TxOut;

    class Payment {

    public:


        Payment();

        Payment(int64_t value, const Coin::P2PKHAddress & addr);

        Payment(int64_t value, const Coin::P2SHAddress & addr);

        Payment(int64_t value, const Coin::PubKeyHash & pkHash);

        // Corresponding transaction output
        Coin::TxOut txOut() const;

        // Getters and setters
        int64_t value() const;
        void setValue(int64_t value);

        uchar_vector serializedScriptPubKey() const;
        void setSerializedScriptPubKey(const uchar_vector & scriptPubKey);

    private:

        // Size of payment (#satoshi)
        int64_t _value;

        // Recipient
        uchar_vector _serializedScriptPubKey;

    };

}

#endif // COMMON_PAYMENT_HPP

