/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_BASE58CHECK_ENCODABLE_HPP
#define COIN_BASE58CHECK_ENCODABLE_HPP

#include <vector>
#include <string>
#include <stdint.h>

#include <common/Network.hpp>

class uchar_vector;

/**
 * Base58Check version bytes
 */

// testnet3
#define P2PKH_ADDRESS_TESTNET3_VERSION_VALUE        0x6F        // dec: 111, base58: m or n ?
#define P2SH_ADDRESS_TESTNET3_VERSION_VALUE         0xC4        // dec: 196, base58: 2
#define WIF_PRIVATE_KEY_TESTNET3_VERSION_VALUE      0xEF        // dec: 239, base58: 9 == uncompressed pubkey, c == compressed pubkey
#define EXTENDED_PRIVATE_KEY_TESTNET3_VERSION_VALUE 0x04358394  // dec: 453131148, base58: tprv
#define EXTENDED_PUBLIC_KEY_TESTNET3_VERSION_VALUE  0x043587CF  // dec: 453135207, base58: tpub

// mainnet
#define P2PKH_ADDRESS_MAINNET_VERSION_VALUE         0x0         // dec: 0, base58: 1
#define P2SH_ADDRESS_MAINNET_VERSION_VALUE          0x5         // dec: 5, base58: 3
#define WIF_PRIVATE_KEY_MAINNET_VERSION_VALUE       0x80        // dec: 128, base58: 5
#define EXTENDED_PRIVATE_KEY_MAINNET_VERSION_VALUE  0x0488ADE4  // dec: 4136173228, base58: xprv
#define EXTENDED_PUBLIC_KEY_MAINNET_VERSION_VALUE   0x0488B21E  // dec: 413617830, base58: xpub

/**
 * Base58Check payload sizes
 */

#define P2PKH_ADDRESS_PAYLOAD_LENGTH                                20
#define P2SH_ADDRESS_PAYLOAD_LENGTH                                 20
#define WIF_PRIVATE_KEY_FOR_UNCOMPRESSED_PUBKEY_PAYLOAD_LENGTH      32 // corresponding to uncompressed public key
#define WIF_PRIVATE_KEY_FOR_COMPRESSED_PUBKEY_PAYLOAD_LENGTH        (32+1) // corresponding to compressed public key
#define EXTENDED_PRIVATE_KEY_PAYLOAD_LENGTH                         2 // change later
#define EXTENDED_PUBLIC_KEY_PAYLOAD_LENGTH                          1 // change later

#define WIF_PRIVATE_KEY_FOR_COMPRESSED_PUBKEY_BYTE  0x01 // Indicates that wif private key corresponds to compressed pubkey

namespace Coin {

// Base58CheckEncodable types which can be determined
// only from version bytes
enum class Base58CheckEncodable {

    P2PKH_ADDRESS, // bip ?
    P2SH_ADDRESS, // bip13

    // There are two different wif formats, depending on
    // if public key is compressed or not, but this cannot distinguished
    // derived from version bytes.
    WIF_PRIVATE_KEY, // bip ?

    EXTENDED_PRIVATE_KEY, // bip32
    EXTENDED_PUBLIC_KEY // bip32

    //ENCRYPTED_PRIVATE_KEY // bip38
};

// Turn type and network to version byte
uint32_t toVersionValue(Base58CheckEncodable encodable, Network network);
std::vector<unsigned char> toVersionBytes(Base58CheckEncodable encodable, Network network);

/**
 * Cannot use, since payload length does not depend on version bytes, e.g. wif keys.
// Turn type into length of corresponding payload
//uint32_t toPayloadLength(Base58CheckEncodable encodable);
*/

// Decode version bytes into type and network
void fromVersionBytes(uint32_t version, Base58CheckEncodable & encodable, Network & network);

// Decode the fom the base58check encoding into the given
// Last three arguments are write locations for result, but they are only modified
// if decoding is successfull, otherwise they are not, and an exception is thrown.
void decodeBase58CheckEncoding(const std::string & encoded, Base58CheckEncodable & encodedType, Network & network, uchar_vector & payload);

}

#endif // COIN_BASE58CHECK_ENCODABLE_HPP
