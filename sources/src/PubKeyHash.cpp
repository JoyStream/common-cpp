/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#include <common/PubKeyHash.hpp>
#include <CoinCore/CoinNodeData.h> // TxOut

namespace Coin {

PubKeyHash::PubKeyHash()
    : UCharArray<PUBKEY_HASH_BYTE_LENGTH>() {
}

PubKeyHash::PubKeyHash(const uchar_vector & raw)
    : UCharArray<PUBKEY_HASH_BYTE_LENGTH>(raw) {
}

}
