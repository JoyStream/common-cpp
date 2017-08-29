/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/MultisigScriptPubKey.hpp>
#include <common/Utilities.hpp> // opPushData
#include <common/P2SHScriptPubKey.hpp>

namespace Coin {

MultisigScriptPubKey::MultisigScriptPubKey(const std::vector<PublicKey> & keys, uint mininumNumberOfSignatures)
    : _keys(keys)
    , _mininumNumberOfSignatures(mininumNumberOfSignatures) {

    // Check inputs are compatible
    if (mininumNumberOfSignatures == 0
        || keys.size() > 16
        || mininumNumberOfSignatures > keys.size())
        throw std::runtime_error("Invalid parameters.");
}

// Raw output script
uchar_vector MultisigScriptPubKey::serialized() const {

    // Copied from Script::Script(type_t type, unsigned int minsigs, const std::vector<bytes_t>& pubkeys, const std::vector<bytes_t>& sigs)

    // Build raw redeem script
    uchar_vector serialized;

    // Add m
    serialized.push_back(_mininumNumberOfSignatures + 0x50);

    // Add each valid raw key
    for(std::vector<PublicKey>::const_iterator i = _keys.cbegin(), end = _keys.cend(); i != end; i++) {
        serialized += opPushData(PublicKey::compressedLength());
        serialized += (*i).toCompressedRawVector();
    }

    // Add n
    serialized.push_back(_keys.size() + 0x50);

    // Checking
    serialized.push_back(0xae); // OP_CHECKMULTISIG

    return serialized;
}

/**
 * Code script hash production code shold not be duplicated
 * this is in redeemscripthash class now
RedeemScriptHash MultisigScriptPubKey::scriptHash() const {
    return RedeemScriptHash(ripemd160(sha256(serialized())));
}
*/

P2SHScriptPubKey MultisigScriptPubKey::toP2SHScriptPubKey() const {
    return P2SHScriptPubKey(RedeemScriptHash::fromRawScript(serialized()));
}

}
