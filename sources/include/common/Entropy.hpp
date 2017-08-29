/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 11 2015
 */

#ifndef ENTROPY_HPP
#define ENTROPY_HPP

#include <common/UCharArray.hpp>

#define WALLET_ENTROPY_BYTE_LENGTH 16 //16 bytes == 128 bits

namespace Coin {
    class Seed;

class Entropy : public Coin::UCharArray<WALLET_ENTROPY_BYTE_LENGTH> {

public:

    // Default constructor
    Entropy();

    // Named Constructor from raw data
    static Entropy fromRaw(const std::vector<unsigned char>&);

    // Named Constructor from raw hex encoded string
    static Entropy fromRawHex(const std::string & hexEncoded);

    std::vector<unsigned char> toRawVector() const;

    static Entropy generate();

    static Entropy fromMnemonic(std::string wordList);

    // BIP39 Seed
    Seed seed(std::string passphrase = "") const;

    // Entropy encoded as BIP39 mnemonic word list
    std::string mnemonic() const;

    // Secure destructor
    ~Entropy();
};

}

#endif // SEED_HPP
