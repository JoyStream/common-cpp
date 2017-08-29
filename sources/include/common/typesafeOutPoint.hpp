/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_typesafeOUT_POINT_HPP
#define COIN_typesafeOUT_POINT_HPP

#include <string>
#include <istream>
#include <ostream>

#include <common/TransactionId.hpp>

namespace Coin {

class OutPoint;

class typesafeOutPoint
{
public:

    // Default constructor
    typesafeOutPoint();

    // Constructor
    typesafeOutPoint(const TransactionId & transactionId, uint32_t index);

    // Copy constructor
    typesafeOutPoint(const typesafeOutPoint& outPoint);

    typesafeOutPoint(const OutPoint & outPoint);

    // Serialized length
    static uint length();

    // Assignment operator
    typesafeOutPoint & operator=(const typesafeOutPoint& outputPoint);

    // Comparison for use with QMap
    friend bool operator==(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);
    friend bool operator!=(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);
    friend bool operator<(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);

    // Stream i/o operators
    friend std::ostream & operator<<(std::ostream & stream, const typesafeOutPoint &);
    friend std::istream & operator>>(std::istream & stream, typesafeOutPoint &);

    // Flat representation, so outpoint can be used as json key
    typesafeOutPoint(const std::string & string);

    std::string toLittleEndianTxIdString() const;

    Coin::OutPoint getClassicOutPoint() const;

    // Getters and setters
    TransactionId transactionId() const;
    void setTransactionId(const TransactionId & transactionId);

    uint32_t index() const;
    void setIndex(uint32_t index);

private:

    // Transaction id of transaction
    TransactionId _txId;

    // Output index
    uint32_t _index;
};

}

uint qHash(const Coin::typesafeOutPoint & o);

#endif // COIN_typesafeOUT_POINT_HPP
