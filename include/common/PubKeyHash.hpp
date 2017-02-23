/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#ifndef COIN_PUBKEY_HASH_HPP
#define COIN_PUBKEY_HASH_HPP

#include <common/UCharArray.hpp>

#define PUBKEY_HASH_BYTE_LENGTH 20

namespace Coin {

typedef UCharArray<PUBKEY_HASH_BYTE_LENGTH> PubKeyHash;

}

#endif // COIN_PUBKEY_HASH_HPP

