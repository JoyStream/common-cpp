/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_NETWORK_HPP
#define COIN_NETWORK_HPP

namespace Coin {

/**
 * Bitcoin network
 */
enum class Network {
    // Bitcoin
    testnet3,
    mainnet,
    regtest,

    // Bitcoin Cash
    testnet_bitcoin_cash,
    mainnet_bitcoin_cash,
    regtest_bitcoin_cash
};

/**
 * Conversion routines
 */

const char * nameFromNetwork(Network network);
Network networkFromName(const char * name);

}

#endif // COIN_NETWORK_HPP
