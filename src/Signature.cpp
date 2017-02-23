/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#include <common/Utilities.hpp>
#include <common/Signature.hpp>

#include <sstream>

namespace Coin {

Signature::Signature()
    : _raw(maxLength, 0) {
}

Signature Signature::fromRawDER(const std::vector<unsigned char> & raw) {
    Signature sig;
    sig.setRawDER(raw);
    return sig;
}

Signature Signature::fromRawDERHex(const std::string& hex) {
    auto raw = Coin::fromHex(hex);
    Signature sig;
    sig.setRawDER(raw);
    return sig;
}

Signature::Signature(const Signature & signature)
    : _raw(signature.rawDER()) {
}

Signature::~Signature() {
    _raw.clear();
}

Signature & Signature::operator=(const Signature & signature) {
    _raw = signature.rawDER();
    return *this;
}

bool Signature::operator==(const Signature & rhs) const {
    return _raw == rhs.rawDER();
}

unsigned int Signature::length() const {
    return _raw.size();
}

std::vector<unsigned char> Signature::rawDER() const {
    return _raw;
}

void Signature::setRawDER(const std::vector<unsigned char> & raw) {
    // Check that input is no longer than
    auto inputLength = raw.size();

    if(inputLength > maxLength) {

        // Create error message and throw exception
        std::stringstream s;

        s << "Input argument must be no longer than"
          << maxLength
          << ", but was of length "
          << inputLength;

        throw std::runtime_error(s.str());
    }

    _raw = raw;
}

}
