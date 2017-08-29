/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/UnspentOutput.hpp>
#include <CoinCore/CoinNodeData.h>

namespace Coin {

UnspentOutput::UnspentOutput()
    : _value(0) {
}

UnspentOutput::UnspentOutput(const typesafeOutPoint & output, uint64_t value)
    : _outPoint(output)
    , _value(value) {
}

bool UnspentOutput::operator==(const UnspentOutput & o) const {

    return _value == o.value() &&
           _outPoint == o.outPoint();
}

bool UnspentOutput::operator!=(const UnspentOutput & o) const {
    return !(*this == o);
}

typesafeOutPoint UnspentOutput::outPoint() const {
    return _outPoint;
}

void UnspentOutput::setOutPoint(const typesafeOutPoint &outPoint) {
    _outPoint = outPoint;
}

uint64_t UnspentOutput::value() const {
    return _value;
}

void UnspentOutput::setValue(uint64_t value) {
    _value = value;
}

uint32_t UnspentOutput::spendingInputSequenceNumber() const {
    return 0xFFFFFFFF; //DEFAULT_SEQUENCE_NUMBER
}

Coin::TxIn UnspentOutput::unsignedSpendingInput() const {
    return Coin::TxIn(outPoint().getClassicOutPoint(), uchar_vector(), spendingInputSequenceNumber());
}
}
