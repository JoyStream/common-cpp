/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef P2PKH_SCRIPT_SIG_HPP
#define P2PKH_SCRIPT_SIG_HPP

#include <common/PublicKey.hpp>
#include <common/TransactionSignature.hpp>

namespace Coin {

class P2PKHScriptSig {

public:

    P2PKHScriptSig(const PublicKey & pk, const TransactionSignature & ts);

    // Maximum possible byte length when serializeds
    static uint32_t maxLength();

    uchar_vector serialized() const;

    static P2PKHScriptSig deserialize(const uchar_vector & script);

    // Getters and setters
    PublicKey pk() const;
    void setPk(const PublicKey & pk);

    TransactionSignature ts() const;
    void setTs(const TransactionSignature & ts);

private:

    PublicKey _pk;

    TransactionSignature _ts;
};

}

#endif // P2PKH_SCRIPT_SIG_HPP

