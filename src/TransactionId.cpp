/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <common/TransactionId.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <algorithm>

namespace Coin {

TransactionId::TransactionId() {
}

uint TransactionId::length() {
    return TXID_BYTE_LENGTH;
}

TransactionId TransactionId::fromTx(const Coin::Transaction & tx) {

    uchar_vector v(tx.getHashLittleEndian());

    return TransactionId::fromRPCByteOrder(v);
}

/**
TransactionId::TransactionId(const Coin::Transaction & tx)
    : UCharArray<TXID_BYTE_LENGTH>(tx.getHashLittleEndian()) { // we need RPC bye order, which coincore calls little endian, *incorrectly*
}
*/

TransactionId TransactionId::fromInternalByteOrder(const std::vector<unsigned char> & vector) {

    // Create blank txid
    TransactionId id;

    id.setRaw(vector);

    // reverse the byte order
    id.reverse();

    return id;
}

TransactionId TransactionId::fromRPCByteOrder(const std::string & str) {

    // Create blank txid
    TransactionId id;

    id.setRawHex(str);

    return id;
}

TransactionId TransactionId::fromRPCByteOrder(const std::vector<unsigned char> & vector) {

    // Create blank txid
    TransactionId id;

    id.setRaw(vector);

    return id;
}

std::string TransactionId::toRPCByteOrder() const {

    return getRawHex();
}

std::vector<unsigned char> TransactionId::toRPCByteOrderVector() const {
    return getRawVector();
}

}
