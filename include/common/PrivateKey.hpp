/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_PRIVATE_KEY_HPP
#define COIN_PRIVATE_KEY_HPP

#include <common/UCharArray.hpp>
#include <common/Network.hpp>

#define PRIVATE_KEY_BYTE_LENGTH 32

namespace Coin {

class PublicKey;
class Signature;
class TransactionSignature;
class Transaction;
enum class PublicKeyCompression;
class SigHashType;

class PrivateKey : public UCharArray<PRIVATE_KEY_BYTE_LENGTH> {

public:

    // Default constructor with blank key
    PrivateKey();

    // Named constructor from raw hex encoded string
    static PrivateKey fromRawHex(const std::string &);

    // Named constructors from raw key
    static PrivateKey fromRaw(const std::vector<unsigned char>&);

    std::vector<unsigned char> toRawVector() const;

    // Secure destruction private key
    ~PrivateKey();

    // Generates fresh key
    static PrivateKey generate();

    // Validity of key
    static bool valid(const PrivateKey &);

    // Sign raw data
    Signature sign(const uchar_vector & message) const;

    // Generates the correponding (compressed) public key
    PublicKey toPublicKey() const;
};

class InvalidPrivateKeyException : std::runtime_error {

public:

    InvalidPrivateKeyException(const PrivateKey & sk)
        : std::runtime_error("Invalid private key.")
        , _sk(sk) {
    }

    PrivateKey sk() const {
        return _sk;
    }

private:

    PrivateKey _sk;
};


}

#endif // COIN_PRIVATE_KEY_HPP
