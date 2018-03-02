/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 24 2015
 */

#include <common/Network.hpp>
#include <string>
#include <stdexcept>
#include <string.h> // strcmp
#include <cassert>

namespace Coin {

const char * nameFromNetwork(Network network) {

    switch(network) {
        case Network::mainnet: return "mainnet";
        case Network::testnet3: return "testnet3";
        case Network::regtest: return "regtest";
        case Network::mainnet_bitcoin_cash: return "mainnet_bitcoin_cash";
        case Network::testnet_bitcoin_cash: return "testnet_bitcoin_cash";
        case Network::regtest_bitcoin_cash: return "regtest_bitcoin_cash";
        default:
            assert(false);
    }

    return "";
}

Network networkFromName(const char * name) {

    if(strcmp(name, "mainnet") == 0)
        return Network::mainnet;
    else if(strcmp(name, "testnet3") == 0)
        return Network::testnet3;
    else if(strcmp(name, "regtest") == 0)
        return Network::regtest;
    else if(strcmp(name, "mainnet_bitcoin_cash") == 0)
        return Network::mainnet_bitcoin_cash;
    else if(strcmp(name, "testnet_bitcoin_cash") == 0)
        return Network::testnet_bitcoin_cash;
    else if(strcmp(name, "regtest_bitcoin_cash") == 0)
        return Network::regtest_bitcoin_cash;
    else
        throw std::runtime_error(std::string("network name unknown: ") + name);

}

}
