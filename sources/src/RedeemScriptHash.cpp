/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#include <common/RedeemScriptHash.hpp>
#include <CoinCore/hash.h>

namespace Coin {

RedeemScriptHash::RedeemScriptHash()
    : UCharArray() {
}

RedeemScriptHash::RedeemScriptHash(const Script &script) {
    setHash(ripemd160(sha256(script.serialize())));
}

RedeemScriptHash RedeemScriptHash::fromRawHash(const std::vector<unsigned char> &hash) {
    RedeemScriptHash rsh;
    rsh.setHash(hash);
    return rsh;
}

RedeemScriptHash RedeemScriptHash::fromRawScript(const std::vector<unsigned char> &script) {
    uchar_vector hash(ripemd160(sha256(script)));
    return fromRawHash(hash);
}

std::vector<unsigned char> RedeemScriptHash::toRawVector() const {
    return getRawVector();
}

void RedeemScriptHash::setHash(const std::vector<unsigned char> &hash) {
    setRaw(hash);
}

}

