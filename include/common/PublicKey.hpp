/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_PUBLIC_KEY_HPP
#define COIN_PUBLIC_KEY_HPP

#include <common/UCharArray.hpp>
#include <common/PubKeyHash.hpp>

#define COMPRESSED_PUBLIC_KEY_BYTE_LENGTH 33
#define UNCOMPRESSED_PUBLIC_KEY_BYTE_LENGTH 65

namespace Coin {

enum class Network;
class Signature;
class P2PKHAddress;

// Compresed public key
class PublicKey : public UCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH> {

public:

    // Defualt constructor
    PublicKey();

    // Named constructor from raw hex encoded string
    static PublicKey fromCompressedRawHex(const std::string &);

    // Named constructors from raw key
    static PublicKey fromCompressedRaw(const std::vector<unsigned char>&);

    static size_t compressedLength();

    std::vector<unsigned char> toCompressedRawVector() const;

    // Whether signature <sig> is valid for the private key corresponding
    // public key <pk> on message <message>
    bool verify(const uchar_vector & message, const Signature & sig) const;

    // Validity of key
    static bool valid(const PublicKey &);

    // Public key hash, e.g. for addresses
    PubKeyHash toPubKeyHash() const;

    // Generate corresponding ntwork
    P2PKHAddress toP2PKHAddress(Network network) const;
};

class InvalidPublicKeyException : std::runtime_error {

public:

    InvalidPublicKeyException(const PublicKey & pk)
        : std::runtime_error("Invalid public key.")
        , _pk(pk) {
    }

    PublicKey pk() const {
        return _pk;
    }

private:

    PublicKey _pk;
};

}

//uint qHash(const Coin::PublicKey & o);

#endif // COIN_PUBLIC_KEY_HPP
