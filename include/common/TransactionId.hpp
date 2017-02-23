/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#ifndef COIN_TRANSACTION_ID_HPP
#define COIN_TRANSACTION_ID_HPP

#include <common/UCharArray.hpp>

#define TXID_BYTE_LENGTH 32

namespace Coin {

    class Transaction;

    // Transaction id using reversed byte order (i.e. block explorer format,
    // this is used since CoinCore & BlockCypher both use it, and its easier for debugging.
    //
    // But here is some more information:
    // ==================================
    // Internal byte order transaction id, incorrectly referred to as
    // "little-endian", is used on wire in outpoints, merkletrees, bloom filters, etc.
    // This should be distinguished from reversed byte order transaction id, incorrectly
    // referred to as "big endian", which is used by by blockexplorers
    // when referencing an individual transaction.
    // More here: https://github.com/bitcoin-dot-org/bitcoin.org/issues/580
    //
    class TransactionId : public UCharArray<TXID_BYTE_LENGTH> {

    public:

        TransactionId();

        // Serialized length
        static uint length();

        // This is safe way to create id, as user does not need to consider endianness
        //explicit TransactionId(const Coin::Transaction & tx);
        static TransactionId fromTx(const Coin::Transaction & tx);

        // Factory from internal byte order
        static TransactionId fromInternalByteOrder(const std::vector<unsigned char> & vector);

        // Factory from hex encoded RPC byte order
        static TransactionId fromRPCByteOrder(const std::string & str);
        static TransactionId fromRPCByteOrder(const std::vector<unsigned char> &vector);

        // Encodes as hex of RPC byte order
        std::string toRPCByteOrder() const;

        std::vector<unsigned char> toRPCByteOrderVector() const;
    };

}

//Q_DECLARE_METATYPE(Coin::TransactionId)

#endif // COIN_TRANSACTION_ID_HPP
