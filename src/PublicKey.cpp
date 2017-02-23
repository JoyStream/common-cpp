/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/Signature.hpp>

#include <CoinCore/hash.h> // ripemd160(sha256(pubkey))
#include <CoinCore/secp256k1_openssl.h> // CoinCrypto::secp256k1_key

namespace Coin {

PublicKey::PublicKey() : UCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH>() {
}

PublicKey PublicKey::fromCompressedRawHex(const std::string &hex) {
    PublicKey pk;

    pk.setRawHex(hex);

    if(!valid(pk))
        throw InvalidPublicKeyException(pk);

    return pk;
}

PublicKey PublicKey::fromCompressedRaw(const std::vector<unsigned char>& raw) {
    PublicKey pk;

    pk.setRaw(raw);

    if(!valid(pk))
        throw InvalidPublicKeyException(pk);

    return pk;
}

size_t PublicKey::compressedLength() {
    return COMPRESSED_PUBLIC_KEY_BYTE_LENGTH;
}

std::vector<unsigned char> PublicKey::toCompressedRawVector() const {
    return getRawVector();
}

PubKeyHash PublicKey::toPubKeyHash() const {
    return PubKeyHash(ripemd160(sha256(toCompressedRawVector())));
}

bool PublicKey::verify(const uchar_vector & message, const Signature & sig) const {

    // Verify key validity: is required due to default constructor,
    // or direct access to UCharArray<>
    if(!valid(*this))
        throw InvalidPublicKeyException(*this);

    // Create signature checking key for thisp public key
    CoinCrypto::secp256k1_key signatureCheckingKey;
    signatureCheckingKey.setPubKey(toCompressedRawVector());

    // Check signature
    bool verified;

    try {

        verified = CoinCrypto::secp256k1_verify(signatureCheckingKey, message, sig.rawDER(), CoinCrypto::SignatureFlag::SIGNATURE_ENFORCE_LOW_S);

    } catch(const std::runtime_error & e) {

        // We may have error if the input was rejected by openssl
        return false;
    }

    return verified;
}

bool PublicKey::valid(const PublicKey & pk) {

    try {

        CoinCrypto::secp256k1_key checkingKey;
        checkingKey.setPubKey(pk.toCompressedRawVector());

        return true;
    } catch (const std::runtime_error &) {
        return false;
    }
}

P2PKHAddress PublicKey::toP2PKHAddress(Network network) const{
    return P2PKHAddress(network, toPubKeyHash());
}

}
