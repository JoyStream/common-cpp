/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 11 2015
 */

#include <common/Seed.hpp>
#include <CoinCore/hdkeys.h>
#include <CoinCore/bip39.h>

#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

namespace Coin {

Seed::Seed() : Coin::UCharArray<WALLET_SEED_BYTE_LENGTH>() {
}

Seed Seed::fromRaw(const std::vector<unsigned char> &raw) {
    Seed s;
    s.setRaw(raw);
    return s;
}

Seed Seed::fromRawHex(const std::string & hex) {
    Seed s;
    s.setRawHex(hex);
    return s;
}

std::vector<unsigned char> Seed::toRawVector() const {
    return getRawVector();
}

Seed::~Seed(){
    clear();
}

Seed Seed::generate() {

    // Seed to fill with entropy
    Seed s;

    if (!RAND_bytes(&s.at(0), WALLET_SEED_BYTE_LENGTH)) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), NULL));
    }

    return s;
}

uint Seed::length() noexcept {
    return WALLET_SEED_BYTE_LENGTH;
}


Coin::HDKeychain Seed::generateHDKeychain() {

    // Create keychain
    Coin::HDSeed hdSeed(this->toRawVector());
    return Coin::HDKeychain(hdSeed.getMasterKey(), hdSeed.getMasterChainCode());
}

/**
void Seed::startAsyncGenerating() {

}

void Seed::stopAsyncGenerating() {

}
*/

}
