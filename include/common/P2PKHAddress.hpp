/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#ifndef COIN_P2PKH_ADDRESS_HPP
#define COIN_P2PKH_ADDRESS_HPP

#include <string>

#include <common/PubKeyHash.hpp>

namespace Coin {

enum class Network;

class P2PKHAddress {

public:

    // Default constructor
    P2PKHAddress();

    // Constructor from public keys
    P2PKHAddress(Network network, const PubKeyHash & pubKeyHash);

    // Factory from Base58CheckEncoding
    static P2PKHAddress fromBase58CheckEncoding(const std::string & encoded);

    // Equality operator
    bool operator==(const P2PKHAddress & o);

    // Base58CheckEncode
    std::string toBase58CheckEncoding() const;

    // Getters and setters
    Network network() const;
    void setNetwork(Network network);

    PubKeyHash pubKeyHash() const;
    void setPubKeyHash(const PubKeyHash & pubKeyHash);

private:

    // Network to which this address corresponds
    Network _network;

    // Pub key hash
    PubKeyHash _pubKeyHash;
};

}

#endif // COIN_P2PKH_ADDRESS_HPP
