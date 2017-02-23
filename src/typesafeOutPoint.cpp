/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/typesafeOutPoint.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::OutPoint

namespace Coin {

typesafeOutPoint::typesafeOutPoint()
    : _index(0){
}

typesafeOutPoint::typesafeOutPoint(const TransactionId & txId, uint32_t index)
    : _txId(txId)
    , _index(index) {
}

typesafeOutPoint::typesafeOutPoint(const typesafeOutPoint & o)
    : typesafeOutPoint(o.transactionId(), o.index()) {
}

typesafeOutPoint::typesafeOutPoint(const OutPoint & outPoint)
    : _txId(Coin::TransactionId::fromRPCByteOrder(uchar_vector(outPoint.hash, Coin::TransactionId::length()))) // TransactionId uses same byte order as coincore
    , _index(outPoint.index) {
}

uint typesafeOutPoint::length() {
    return TransactionId::length() + sizeof(uint32_t);
}

typesafeOutPoint & typesafeOutPoint::operator=(const typesafeOutPoint& o) {

    _txId = o.transactionId();
    _index = o.index();

    return *this;
}

bool operator==(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return (lhs.transactionId() == rhs.transactionId()) && (lhs.index() == rhs.index());
}

bool operator!=(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return !(lhs == rhs);
}

bool operator<(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return (lhs.transactionId() < rhs.transactionId()) || ((lhs.transactionId() == rhs.transactionId()) && (lhs.index() < rhs.index()));
}

std::ostream & operator<<(std::ostream & stream, const typesafeOutPoint & o) {
    stream << o._txId << o._index;

    return stream;
}

std::istream & operator>>(std::istream & stream, typesafeOutPoint & o) {
    stream >> o._txId >> o._index;

    return stream;
}

std::string typesafeOutPoint::toLittleEndianTxIdString() const {
    return _txId.toRPCByteOrder() + "-" + std::to_string(_index);
}

Coin::OutPoint typesafeOutPoint::getClassicOutPoint() const {
    return Coin::OutPoint(_txId.toRPCByteOrderVector(), _index); // TransactionId uses same byte order as coincore
}

TransactionId typesafeOutPoint::transactionId() const {
    return _txId;
}

void typesafeOutPoint::setTransactionId(const TransactionId & txId){
    _txId = txId;
}

uint32_t typesafeOutPoint::index() const {
    return _index;
}

void typesafeOutPoint::setIndex(uint32_t index) {
    _index = index;
}

}

uint qHash(const Coin::typesafeOutPoint & o) {

    uchar_vector total = o.transactionId().toRPCByteOrderVector();
    total.push_back((unsigned char)o.index());

    std::hash<std::string> hexHash;
    return hexHash(total.getCharsAsString());
}
