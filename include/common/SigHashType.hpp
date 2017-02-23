/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#include <stdint.h>

#ifndef COMMON_SIGHASHTYPE_HPP
#define COMMON_SIGHASHTYPE_HPP

namespace Coin {

#define SIGHASHTYPE_BITMASK 0x1f

#define SIGHASH_ALL 0x01
#define SIGHASH_NONE 0x02
#define SIGHASH_SINGLE 0x03
#define SIGHASH_ANYONECANPAY 0x80

class SigHashType {

public:

    // You can only be have one of these
    enum class MutuallyExclusiveType {
        all,
        none,
        single
    };

    SigHashType();

    SigHashType(MutuallyExclusiveType type, bool anyOneCanPay);

    bool operator==(const SigHashType & rhs);

    // From (little endian) hash code, as it appears in sighash computation step
    static SigHashType fromHashCode(uint32_t hashCode);

    // Whether (little endian) hash code has anyonecan pay flag set
    static bool canAnyonePay(uint32_t hashCode);

    // Returns type all and anyonecanpay
    static SigHashType standard();

    // Is of type all and !anyonecanpay
    bool isStandard() const;

    // Returns zeroed out byte with flag set for *only* given hash type
    static unsigned char flag(MutuallyExclusiveType type);

    // To (little endian) hash code, as it appears in sighash computation step
    unsigned char hashCode() const;

    // Getters and setters
    MutuallyExclusiveType type() const;
    void setType(MutuallyExclusiveType type);

    bool anyOneCanPay() const;
    void setAnyOneCanPay(bool anyOneCanPay);

private:

    MutuallyExclusiveType _type;

    bool _anyOneCanPay;

};

}

#endif // COMMON_SIGHASHTYPE_HPP
