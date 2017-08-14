/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#include <string>
#include <istream>
#include <ostream>
#include <stdutils/uchar_vector.h>

#ifndef COIN_SIGNATURE_HPP
#define COIN_SIGNATURE_HPP

namespace Coin {

// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR

class Signature
{
public:

    // The maximum byte length of a valid signature
    const static unsigned int maxLength = 73;

    // Default constructor
    Signature();

    // Constructor from members
    static Signature fromRawDER(const std::vector<unsigned char> &);

    static Signature fromRawDERHex(const std::string&);

    // Copy constructor
    Signature(const Signature & signature);

    // Safe destructor
    ~Signature();

    // Assignment operator
    Signature & operator=(const Signature & signature);

    // Equality test
    bool operator==(const Signature & rhs) const;

    // Length of signature
    unsigned int length() const;

    // Getters and setters
    std::vector<unsigned char> rawDER() const;

private:
    void setRawDER(const std::vector<unsigned char> &raw);

    // (Strict?) DER encoded signature
    // 0 is most significant byte for comparisons
    std::vector<unsigned char> _raw;
};

}

#endif // COIN_SIGNATURE_HPP

