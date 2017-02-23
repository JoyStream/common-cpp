/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#include <common/Base58CheckEncodable.hpp>

#include <CoinCore/Base58Check.h>
#include <stdutils/uchar_vector.h>

#include <cassert>
#include <sstream> // stringstream

//#include <CoinCore/CoinNodeData.h>

namespace Coin {

// Based on https://en.bitcoin.it/wiki/List_of_address_prefixes
uint32_t toVersionValue(Base58CheckEncodable encodable, Network network) {

    //Coin::Transaction id;

    // testnet3 and regtest address version numbers are identical
    if(network == Network::testnet3 || network == Network::regtest) {

        switch(encodable) {

            case Base58CheckEncodable::P2PKH_ADDRESS:           return P2PKH_ADDRESS_TESTNET3_VERSION_VALUE;
            case Base58CheckEncodable::P2SH_ADDRESS:            return P2SH_ADDRESS_TESTNET3_VERSION_VALUE;
            case Base58CheckEncodable::WIF_PRIVATE_KEY:         return WIF_PRIVATE_KEY_TESTNET3_VERSION_VALUE;
            case Base58CheckEncodable::EXTENDED_PRIVATE_KEY:    return EXTENDED_PRIVATE_KEY_TESTNET3_VERSION_VALUE;
            case Base58CheckEncodable::EXTENDED_PUBLIC_KEY:     return EXTENDED_PUBLIC_KEY_TESTNET3_VERSION_VALUE;
            default:
                assert(false);
        }

    } else if (network == Network::mainnet) {

        switch(encodable) {

            case Base58CheckEncodable::P2PKH_ADDRESS:           return P2PKH_ADDRESS_MAINNET_VERSION_VALUE;
            case Base58CheckEncodable::P2SH_ADDRESS:            return P2SH_ADDRESS_MAINNET_VERSION_VALUE;
            case Base58CheckEncodable::WIF_PRIVATE_KEY:         return WIF_PRIVATE_KEY_MAINNET_VERSION_VALUE;
            case Base58CheckEncodable::EXTENDED_PRIVATE_KEY:    return EXTENDED_PRIVATE_KEY_MAINNET_VERSION_VALUE;
            case Base58CheckEncodable::EXTENDED_PUBLIC_KEY:     return EXTENDED_PUBLIC_KEY_MAINNET_VERSION_VALUE;
            default:
                assert(false);
        }

    } else
        assert(false);
}

std::vector<unsigned char> toVersionBytes(Base58CheckEncodable encodable, Network network) {

    // Recover version value
    uint32_t versionValue = toVersionValue(encodable, network);

    // Encode as bytes
    unsigned char * p = (unsigned char *)(&versionValue);

    std::vector<unsigned char> versionBytes;

    versionBytes.push_back(p[0]);

    // WIF private keys have four byte version bytes
    if(encodable == Base58CheckEncodable::WIF_PRIVATE_KEY) {
        versionBytes.push_back(p[1]);
        versionBytes.push_back(p[2]);
        versionBytes.push_back(p[3]);
    }

    return versionBytes;
}

void fromVersionBytes(uint32_t version, Base58CheckEncodable & encodable, Network & network) {

    // Temporary variables
    Base58CheckEncodable e;
    Network n;

    switch(version) {

        // testnet3
        case P2PKH_ADDRESS_TESTNET3_VERSION_VALUE:              e = Base58CheckEncodable::P2PKH_ADDRESS; n = Network::testnet3; break;
        case P2SH_ADDRESS_TESTNET3_VERSION_VALUE:               e = Base58CheckEncodable::P2SH_ADDRESS; n = Network::testnet3; break;
        case WIF_PRIVATE_KEY_TESTNET3_VERSION_VALUE:            e = Base58CheckEncodable::WIF_PRIVATE_KEY; n = Network::testnet3; break;
        case EXTENDED_PRIVATE_KEY_TESTNET3_VERSION_VALUE:       e = Base58CheckEncodable::EXTENDED_PRIVATE_KEY; n = Network::testnet3; break;
        case EXTENDED_PUBLIC_KEY_TESTNET3_VERSION_VALUE:        e = Base58CheckEncodable::EXTENDED_PUBLIC_KEY; n = Network::testnet3; break;

        // mainnet
        case P2PKH_ADDRESS_MAINNET_VERSION_VALUE:               e = Base58CheckEncodable::P2PKH_ADDRESS; n = Network::mainnet; break;
        case P2SH_ADDRESS_MAINNET_VERSION_VALUE:                e = Base58CheckEncodable::P2SH_ADDRESS; n = Network::mainnet; break;
        case WIF_PRIVATE_KEY_MAINNET_VERSION_VALUE:             e = Base58CheckEncodable::WIF_PRIVATE_KEY; n = Network::mainnet; break;
        case EXTENDED_PRIVATE_KEY_MAINNET_VERSION_VALUE:        e = Base58CheckEncodable::EXTENDED_PRIVATE_KEY; n = Network::mainnet; break;
        case EXTENDED_PUBLIC_KEY_MAINNET_VERSION_VALUE:         e = Base58CheckEncodable::EXTENDED_PUBLIC_KEY; n = Network::mainnet; break;
        default:
            throw std::runtime_error("Invalid version bytes.");
    }

    // Write return values
    encodable = e;
    network = n;
}

/**
uint32_t toPayloadLength(Base58CheckEncodable encodable) {

    switch(encodable) {

        case Base58CheckEncodable::P2PKH_ADDRESS:           return P2PKH_ADDRESS_PAYLOAD_LENGTH;
        case Base58CheckEncodable::P2SH_ADDRESS:            return P2SH_ADDRESS_PAYLOAD_LENGTH;
        case Base58CheckEncodable::WIF_PRIVATE_KEY:         return WIF_PRIVATE_KEY_PAYLOAD_LENGTH;
        case Base58CheckEncodable::EXTENDED_PRIVATE_KEY:    return EXTENDED_PRIVATE_KEY_PAYLOAD_LENGTH;
        case Base58CheckEncodable::EXTENDED_PUBLIC_KEY:     return EXTENDED_PUBLIC_KEY_PAYLOAD_LENGTH;
        default:
            Q_ASSERT(false);
    }
}
*/

void decodeBase58CheckEncoding(const std::string & encoded, Base58CheckEncodable & encodable, Network & network, uchar_vector & payload) {

    // Buffers for parsing encoded string into
    uchar_vector actualPayload;
    uint32_t version;

    // Try to decode string
    // ----
    // WARNING: WIF PRIVATE KEYS WILL NOT HAVE THEIR 4 BYTE VERSION FIELD PROPERLY DECODED DUE TO MSIGNA BUG
    // ----
    bool isValid = fromBase58Check(encoded, actualPayload, version);

    // Throw error if encoding is not valid
    if(!isValid)
        throw std::runtime_error("Malformed encoding.");

    // Decode version bytes into type and network
    Base58CheckEncodable actualEncoded;
    Network actualNetwork;

    fromVersionBytes(version, actualEncoded, actualNetwork);

    // Check that payload length corresponds with type
    uchar_vector::size_type actualPayloadLength = actualPayload.size();

    // Treat WIF keys as special, as they can have two different payload lengths
    if(actualEncoded == Base58CheckEncodable::WIF_PRIVATE_KEY) {

        if(actualPayloadLength == WIF_PRIVATE_KEY_FOR_UNCOMPRESSED_PUBKEY_PAYLOAD_LENGTH)
            throw std::runtime_error("not implemented decoding wif encoded private keys for uncompressed public keys");
        else if(actualPayloadLength == WIF_PRIVATE_KEY_FOR_COMPRESSED_PUBKEY_PAYLOAD_LENGTH)
            throw std::runtime_error("not implemented decoding wif encoded private keys for uncompressed public keys");
        else {

            // Create and throw eror message
            std::stringstream s;

            s << "Version bytes indicate payload length of"
              << WIF_PRIVATE_KEY_FOR_UNCOMPRESSED_PUBKEY_PAYLOAD_LENGTH
              << " or "
              << WIF_PRIVATE_KEY_FOR_COMPRESSED_PUBKEY_PAYLOAD_LENGTH
              << " bytes, but actual length was"
              << actualPayloadLength;

            throw std::runtime_error(s.str());
        }

    } else {

        // Deduce expected payload length from version bytes
        uint32_t expectedPayLoadLength;

        switch(actualEncoded) {

            case Base58CheckEncodable::P2PKH_ADDRESS:           expectedPayLoadLength = P2PKH_ADDRESS_PAYLOAD_LENGTH; break;
            case Base58CheckEncodable::P2SH_ADDRESS:            expectedPayLoadLength = P2SH_ADDRESS_PAYLOAD_LENGTH; break;
            case Base58CheckEncodable::WIF_PRIVATE_KEY:         assert(false); // should not be here, treated as sepecial case above
            case Base58CheckEncodable::EXTENDED_PRIVATE_KEY:    expectedPayLoadLength = EXTENDED_PRIVATE_KEY_PAYLOAD_LENGTH; break;
            case Base58CheckEncodable::EXTENDED_PUBLIC_KEY:     expectedPayLoadLength = EXTENDED_PUBLIC_KEY_PAYLOAD_LENGTH; break;
            default:
                assert(false);
        }

        // Check that it is correct
        if(actualPayloadLength != expectedPayLoadLength) {

            // Create and throw eror message
            std::stringstream s;

            s << "Version bytes indicate payload length "
              << expectedPayLoadLength
              << " bytes, but actual length was"
              << actualPayloadLength;

            throw std::runtime_error(s.str());
        }
    }

    // Since everything was valid, we write results to destination variables
    encodable = actualEncoded;
    network = actualNetwork;
    payload = actualPayload;
}

}
