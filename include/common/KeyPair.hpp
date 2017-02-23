/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_KEYPAIR_HPP
#define COIN_KEYPAIR_HPP

#include <common/PublicKey.hpp>
#include <common/PrivateKey.hpp>

//class QJsonObject;

namespace Coin {

class KeyPair
{
public:

    // Default constructor
    KeyPair();

    // Constructor from members
    KeyPair(const PublicKey & pk, const PrivateKey & sk);

    KeyPair(const PrivateKey & sk);

    // Copy constructor
    KeyPair(const KeyPair& keyPair);

    // Generate fresh key pair
    static KeyPair generate();

    // Assignment operator
    KeyPair & operator=(const KeyPair& rhs);

    // Comparison operator
    bool operator==(const KeyPair & o) const;

    /**

    // Constructor from json
    KeyPair(const QJsonObject & json);

    // Converts to json
    QJsonObject json() const;

    */

    // Getters and setters
    PublicKey pk() const;
    void setPk(const PublicKey & pk);

    PrivateKey sk() const;
    void setSk(const PrivateKey & sk);

private:

    // Public key
    PublicKey _pk;

    // Private key
    PrivateKey _sk;
};

}

#endif // COIN_KEYPAIR_HPP
