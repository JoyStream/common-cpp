/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <cassert>

#include <common/P2PKHScriptPubKey.hpp>
#include <common/PublicKey.hpp>

namespace Coin {

P2PKHScriptPubKey::P2PKHScriptPubKey(const PubKeyHash & hash)
    : _hash(hash) {
}

P2PKHScriptPubKey::P2PKHScriptPubKey(const PublicKey & pk)
    : P2PKHScriptPubKey(pk.toPubKeyHash()) {
}

uint32_t P2PKHScriptPubKey::length() {
    return P2PKHScriptPubKey(PubKeyHash()).serialize().size();
}

P2PKHScriptPubKey P2PKHScriptPubKey::deserialize(const uchar_vector & script) {
    // Check the script is a standard p2pkh script
    if(script.size() != 25 || script[0] != 0x76 ||
            script[1] != 0xa9 || script[2] != 0x14 || script[23] != 0x88 || script[24] != 0xac)
        throw std::runtime_error("malformed p2pkh script pubkey");

    // Extract the pubkeyhash from the script (bytes 4 to 24)
    uchar_vector hash(&script.at(3), &script.at(23));

    P2PKHScriptPubKey scriptPubKey = P2PKHScriptPubKey(PubKeyHash(hash));

    assert(scriptPubKey.serialize() == script);

    return scriptPubKey;
}

P2PKHScriptPubKey P2PKHScriptPubKey::deserialize(const std::string & hex) {
    return deserialize(uchar_vector(hex));

}

uchar_vector P2PKHScriptPubKey::serialize() const {

    // Copied from bytes_t Script::txoutscript() const
    uchar_vector script;

    script.push_back(0x76); // OP_DUP
    script.push_back(0xa9); // OP_HASH160
    script.push_back(0x14); // push a 20 byte hash
    script += _hash.getRawVector(); // pubkeyhash
    script.push_back(0x88); // OP_EQUALVERIFY
    script.push_back(0xac); // OP_CHECKSIG

    return script;
}


}
