/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/MultisigScriptSig.hpp>
#include <common/Utilities.hpp> // uchar_vector toSerialized(const std::vector<TransactionSignature> sigs)

namespace Coin {

MultisigScriptSig::MultisigScriptSig(const std::vector<TransactionSignature> sigs)
    : _sigs(sigs) {
}

uchar_vector MultisigScriptSig::serialized() const {

    throw std::runtime_error("has no test coverage");

    uchar_vector serialized;

    serialized += serializeForOP_CHECKSIGMULTISIG(_sigs);

    return serialized;
}

}
