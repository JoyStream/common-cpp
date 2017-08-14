/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#ifndef COMMON_REDEEMSCRIPTHASH_HPP
#define COMMON_REDEEMSCRIPTHASH_HPP

#include <common/UCharArray.hpp>
#include <common/Script.hpp>

#define REDEEM_SCRIPT_HASH_BYTE_LENGTH 20

namespace Coin {

class RedeemScriptHash : public UCharArray<REDEEM_SCRIPT_HASH_BYTE_LENGTH> {
public:
    RedeemScriptHash();

    RedeemScriptHash(const Script &script);

    static RedeemScriptHash fromRawHash(const std::vector<unsigned char> &hash);
    static RedeemScriptHash fromRawScript(const std::vector<unsigned char> &script);

    std::vector<unsigned char> toRawVector() const;

private:
    void setHash(const std::vector<unsigned char> &hash);
};

}

#endif // COMMON_REDEEMSCRIPTHASH_HPP

