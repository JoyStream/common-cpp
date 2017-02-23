/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 7 2015
 */

#include <common/SigHashType.hpp>

#include <stdexcept>

namespace Coin {

SigHashType::SigHashType()
    : _type(MutuallyExclusiveType::all)
    , _anyOneCanPay(false) {
}

SigHashType::SigHashType(MutuallyExclusiveType type, bool anyOneCanPay)
    : _type(type)
    , _anyOneCanPay(anyOneCanPay) {
}

bool SigHashType::operator==(const SigHashType & rhs) {
    return _type == rhs.type() && _anyOneCanPay == rhs.anyOneCanPay();
}

SigHashType SigHashType::fromHashCode(uint32_t hashCode) {

    uint32_t masked = (hashCode & SIGHASHTYPE_BITMASK);

    MutuallyExclusiveType type;

    if(flag(MutuallyExclusiveType::all) == masked)
        type = MutuallyExclusiveType::all;
    else if(flag(MutuallyExclusiveType::none) == masked)
        type = MutuallyExclusiveType::none;
    else if(flag(MutuallyExclusiveType::single) == masked)
        type = MutuallyExclusiveType::single;
    else
        throw std::runtime_error("No standard mandatory sighash flag was set.");

    return SigHashType(type, canAnyonePay(hashCode));
}

bool SigHashType::canAnyonePay(uint32_t hashCode) {
    return hashCode & SIGHASH_ANYONECANPAY;
}

SigHashType SigHashType::standard() {
    return SigHashType(MutuallyExclusiveType::all, false);
}

bool SigHashType::isStandard() const {
    return (_type == MutuallyExclusiveType::all) && !_anyOneCanPay;
}

unsigned char SigHashType::flag(MutuallyExclusiveType type) {

    switch(type) {

        case MutuallyExclusiveType::all: return SIGHASH_ALL;
        case MutuallyExclusiveType::none: return SIGHASH_NONE;
        case MutuallyExclusiveType::single: return SIGHASH_SINGLE;

        default:
            throw std::runtime_error("Coding error, unsupported MutuallyExclusiveType");
    }
}

unsigned char SigHashType::hashCode() const {
    return flag(_type) | ( _anyOneCanPay ? SIGHASH_ANYONECANPAY : 0);
}

SigHashType::MutuallyExclusiveType SigHashType::type() const {
    return _type;
}

void SigHashType::setType(MutuallyExclusiveType type) {
    _type = type;
}

bool SigHashType::anyOneCanPay() const {
    return _anyOneCanPay;
}

void SigHashType::setAnyOneCanPay(bool anyOneCanPay) {
    _anyOneCanPay = anyOneCanPay;
}

}
