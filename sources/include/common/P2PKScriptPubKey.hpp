/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef P2PK_SCRIPT_PUBKEY_HPP
#define P2PK_SCRIPT_PUBKEY_HPP

#include <common/PublicKey.hpp>
#include <common/Script.hpp>
#include <stdutils/uchar_vector.h>

namespace Coin {

class PublicKey;

class P2PKScriptPubKey : public Script {

public:

    P2PKScriptPubKey(const PublicKey & pk);

    static uint32_t length();

    // Raw script: <pubKey> <checkSig>
    uchar_vector serialize() const;

    PublicKey pubKey() const { return _pubKey; }

private:

    PublicKey _pubKey;
};

}

#endif // P2PK_SCRIPT_PUBKEY_HPP

