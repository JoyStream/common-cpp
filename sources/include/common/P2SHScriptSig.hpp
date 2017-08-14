/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef P2SH_SCRIPT_SIG_HPP
#define P2SH_SCRIPT_SIG_HPP

#include <common/TransactionSignature.hpp>
#include <vector>

namespace Coin {

/** NOTE: Make subclass of proper typesafe script class in the future **/
class P2SHScriptSig {

public:

    P2SHScriptSig(const std::vector<TransactionSignature> & sigs, const uchar_vector & redeemScript);

    uchar_vector serialized();

private:

    std::vector<TransactionSignature> _sigs;

    uchar_vector _redeemScript;
};

}

#endif // P2SH_SCRIPT_SIG_HPP

