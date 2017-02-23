/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#include <common/P2SHAddress.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/Base58CheckEncodable.hpp>
#include <CoinCore/Base58Check.h>

namespace Coin {

P2SHAddress::P2SHAddress(Network network, const RedeemScriptHash & redeemScriptHash)
    : _network(network),
      _redeemScriptHash(redeemScriptHash)
{
}

P2SHAddress P2SHAddress::fromBase58CheckEncoding(const std::string & encoded) {

    // Decode
    Base58CheckEncodable encodedType;
    Network network;
    uchar_vector redeemScriptHash;

    decodeBase58CheckEncoding(encoded, encodedType, network, redeemScriptHash);

    // Check that input is indeed a p2pkh address
    if(encodedType != Base58CheckEncodable::P2SH_ADDRESS)
        throw std::runtime_error("Argument was not a P2SH address.");

    // Create address and return
    return P2SHAddress(network, RedeemScriptHash::fromRawHash(redeemScriptHash));
}

std::string P2SHAddress::toBase58CheckEncoding() const {

    // Create version bytes
    std::vector<unsigned char> versionBytes = toVersionBytes(Base58CheckEncodable::P2SH_ADDRESS, _network);

    // Base58Check encode and return result
    std::string encoded = toBase58Check(_redeemScriptHash.toRawVector(), versionBytes);

    return encoded;
}

P2SHScriptPubKey P2SHAddress::toP2SHScriptPubKey() const {
    return P2SHScriptPubKey(_redeemScriptHash);
}

Network P2SHAddress::network() const {
    return _network;
}

void P2SHAddress::setNetwork(Network network) {
    _network = network;
}

RedeemScriptHash P2SHAddress::redeemScriptHash() const {
    return _redeemScriptHash;
}

void P2SHAddress::setRedeemScriptHash(const RedeemScriptHash & redeemScriptHash) {
    _redeemScriptHash = redeemScriptHash;
}

}
