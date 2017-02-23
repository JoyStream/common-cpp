/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/P2PKScriptPubKey.hpp>
#include <common/PublicKey.hpp>

namespace Coin {

P2PKScriptPubKey::P2PKScriptPubKey(const PublicKey & pk)
    : _pubKey(pk) {
}

uint32_t P2PKScriptPubKey::length() {
    return P2PKScriptPubKey(PublicKey()).serialize().size();
}

uchar_vector P2PKScriptPubKey::serialize() const {

    uchar_vector script;

    script.push_back(0x21); // push a 33 byte compressed public key
    script += _pubKey.toCompressedRawVector(); // pubKey
    script.push_back(0xac); // OP_CHECKSIG

    return script;
}


}
