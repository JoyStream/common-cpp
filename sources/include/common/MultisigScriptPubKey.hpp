/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef MULTISIG_SCRIPT_PUBKEY_HPP
#define MULTISIG_SCRIPT_PUBKEY_HPP

#include <common/PublicKey.hpp>
//#include <common/RedeemScriptHash.hpp> // cannot forward declare

namespace Coin {

class P2SHScriptPubKey;

// NOTE: Not same as P2SH (bip16) multisig scriptpubkey,
// this is just standard bip11.

/** NOTE: Make subclass of proper typesafe script class in the future **/
class MultisigScriptPubKey  {

public:

    MultisigScriptPubKey(const std::vector<PublicKey> & keys, uint mininumNumberOfSignatures);

    // Raw output script: m {pubkey}...{pubkey} n OP_CHECKMULTISIG
    uchar_vector serialized() const;

    /**
    Deprecated!
    // Hash of serialzied script
    RedeemScriptHash scriptHash() const;
    */

    // P2SH multisig output script:
    P2SHScriptPubKey toP2SHScriptPubKey() const;

private:

    // Keys
    std::vector<PublicKey> _keys;

    // #sigs
    uint _mininumNumberOfSignatures;

};

}

#endif // MULTISIG_SCRIPT_PUBKEY_HPP

