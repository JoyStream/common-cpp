/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 11 2015
 */

#ifndef SEED_HPP
#define SEED_HPP

#include <string>
#include <vector>

#include <common/UCharArray.hpp>

#define WALLET_SEED_BYTE_LENGTH 64 //64 bytes == 512 bits

// Seed used for wallet, stored in dbase
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR

namespace Coin {
    class HDKeychain;

class Seed : public Coin::UCharArray<WALLET_SEED_BYTE_LENGTH> {

public:

    // Default constructor
    Seed();

    // Named constructor from raw data
    static Seed fromRaw(const std::vector<unsigned char> &);

    // Named constructor from hex string
    static Seed fromRawHex(const std::string&);

    std::vector<unsigned char> toRawVector() const;

    /**
     * TODO
     *
     * 1) ADD SIGNALS AND SLOTS TO MAKE SEED GENERATION ASYNCHRONOUS LATER
     *
     * 2) ADD SOME SORT OF SEEDING OF THE SEED GENERATION, E.G.
     * USER MOUSE INPUT OR WHATEVER
     **/

    // Synchrounously generates new seed, is quite slow?
    static Seed generate();

    // Length of seeds
    static uint length() noexcept;

    // Generate a HD key chain from seed
    Coin::HDKeychain generateHDKeychain();

    // Secure destructor
    ~Seed();

    // Asynchrnous generation routines
    //void startAsyncGenerating();
    //void stopAsyncGenerating();

//signals:

    // Done generating signal
    //void finishedAsyncGenerating(Seed s);
};

}

#endif // SEED_HPP
