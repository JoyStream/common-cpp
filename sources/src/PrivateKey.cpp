/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/PublicKeyCompression.hpp>
#include <common/Base58CheckEncodable.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/Signature.hpp>
#include <common/TransactionSignature.hpp>
#include <common/SigHashType.hpp>
#include <common/Utilities.hpp> // sighash

#include <CoinCore/Base58Check.h>
#include <CoinCore/secp256k1_openssl.h>

#include <string>
#include <cassert>

namespace Coin {

PrivateKey::PrivateKey()
    : UCharArray<PRIVATE_KEY_BYTE_LENGTH>() {
}

PrivateKey PrivateKey::fromRawHex(const std::string &hex) {
    PrivateKey sk;

    sk.setRawHex(hex);

    if(!valid(sk))
        throw InvalidPrivateKeyException(sk);

    return sk;
}

PrivateKey PrivateKey::fromRaw(const std::vector<unsigned char>& raw) {
    PrivateKey sk;

    sk.setRaw(raw);

    if(!valid(sk))
        throw InvalidPrivateKeyException(sk);

    return sk;
}

std::vector<unsigned char> PrivateKey::toRawVector() const {
    return getRawVector();
}

PrivateKey::~PrivateKey() {
    clear();
}

PrivateKey PrivateKey::generate() {

    // Create fresh private keys
    CoinCrypto::secp256k1_key key;

    // Generate key
    key.newKey();

    // Create private key from raw uchar data
    return PrivateKey::fromRaw(key.getPrivKey());
}

bool PrivateKey::valid(const PrivateKey & sk) {

    try {

        CoinCrypto::secp256k1_key checkingKey;
        checkingKey.setPrivKey(sk.toRawVector());

        return true;
    } catch (const std::runtime_error &) {
        return false;
    }
}

Signature PrivateKey::sign(const uchar_vector & data) const {

    // Verify key validity
    if(!valid(*this))
        throw InvalidPrivateKeyException(*this);

    // Create signing key
    CoinCrypto::secp256k1_key signingKey;
    signingKey.setPrivKey(toRawVector());

    // Comute signature and return
    return Signature::fromRawDER(CoinCrypto::secp256k1_sign(signingKey, data));
}

PublicKey PrivateKey::toPublicKey() const {

    // Verify key validity
    if(!valid(*this))
        throw InvalidPrivateKeyException(*this);

    // Wrap in key class
    CoinCrypto::secp256k1_key sk;
    sk.setPrivKey(toRawVector());

    // Convert to compressed public key
    bytes_t publicKey = sk.getPubKey(true);

    return PublicKey::fromCompressedRaw(publicKey);
}

}
