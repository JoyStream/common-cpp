/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_TRANSACTION_MERKLE_TREE_ROOT_HPP
#define COIN_TRANSACTION_MERKLE_TREE_ROOT_HPP

#include <common/UCharArray.hpp>

#define TRANSACTIONMERKLETREEROOT_BYTE_LENGTH 32

namespace Coin {

typedef UCharArray<TRANSACTIONMERKLETREEROOT_BYTE_LENGTH> TransactionMerkleTreeRoot;

}

#endif // COIN_TRANSACTION_MERKLE_TREE_ROOT_HPP
