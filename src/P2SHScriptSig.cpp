/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/P2SHScriptSig.hpp>
#include <common/Utilities.hpp> // uchar_vector toSerialized(const std::vector<TransactionSignature> sigs)


namespace Coin {

P2SHScriptSig::P2SHScriptSig(const std::vector<TransactionSignature> & sigs, const uchar_vector & redeemScript)
    : _sigs(sigs)
    , _redeemScript(redeemScript) {
}

uchar_vector P2SHScriptSig::serialized() {

    uchar_vector serialized;

    serialized += serializeForOP_CHECKSIGMULTISIG(_sigs);
    serialized += opPushData(_redeemScript.size());
    serialized += _redeemScript;

    return serialized;
}

}
