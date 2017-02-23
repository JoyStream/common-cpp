/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#ifndef COIN_P2SH_ADDRESS_HPP
#define COIN_P2SH_ADDRESS_HPP

#include <string>

#include <common/Network.hpp>
#include <common/RedeemScriptHash.hpp>

namespace Coin {

class TxOut;
class P2SHScriptPubKey;

/**
 * Consollidate some how with P2PKHAddress type,
 * they are basically identical.
 */

class P2SHAddress {

public:
    P2SHAddress(Network network, const RedeemScriptHash & redeemScriptHash);

    // Factory from Base58CheckEncoding
    static P2SHAddress fromBase58CheckEncoding(const std::string & encoded);

    // Base58CheckEncode
    std::string toBase58CheckEncoding() const;

    // Transaction output
    TxOut txOut(uint64_t value) const;

    // P2SHScriptPubKey
    P2SHScriptPubKey toP2SHScriptPubKey() const;

    // Getters and setters
    Network network() const;
    void setNetwork(Network network);

    RedeemScriptHash redeemScriptHash() const;
    void setRedeemScriptHash(const RedeemScriptHash & redeemScriptHash);

private:

    Network _network;

    // Hash of serialized redeem script
    RedeemScriptHash _redeemScriptHash;

};

}

#endif // COIN_P2SH_ADDRESS_HPP
