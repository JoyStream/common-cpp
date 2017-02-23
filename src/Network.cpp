/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 24 2015
 */

#include <common/Network.hpp>

#include <string.h> // strcmp
#include <cassert>

namespace Coin {

const char * nameFromNetwork(Network network) {

    switch(network) {
        case Network::mainnet: return "mainnet";
        case Network::testnet3: return "testnet3";
        case Network::regtest: return "regtest";
        default:
            assert(false);
    }

}

Network networkFromName(const char * name) {

    if(strcmp(name, "mainnet") == 0)
        return Network::mainnet;
    else if(strcmp(name, "testnet3") == 0)
        return Network::testnet3;
    else if(strcmp(name, "regtest") == 0)
        return Network::regtest;
    else
        assert(false);

}

}
