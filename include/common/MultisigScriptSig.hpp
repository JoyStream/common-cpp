/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef MULTISIG_SCRIPT_SIG_HPP
#define MULTISIG_SCRIPT_SIG_HPP

#include <common/TransactionSignature.hpp>
#include <vector>

namespace Coin {

// NOTE: Not same as P2SH (bip16) multisig scriptpubkey,
// this is just standard bip11.

/** NOTE: Make subclass of proper typesafe script class in the future **/
class MultisigScriptSig {

public:

    MultisigScriptSig(const std::vector<TransactionSignature> sigs);

    // OP_0 ...signatures...
    uchar_vector serialized() const;

private:

    std::vector<TransactionSignature> _sigs;

};

}

#endif // MULTISIG_SCRIPT_SIG_HPP

