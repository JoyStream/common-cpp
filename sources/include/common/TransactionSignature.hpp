/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef TRANSACTION_SIGNATURE_HPP
#define TRANSACTION_SIGNATURE_HPP

#include <common/Signature.hpp>
#include <common/SigHashType.hpp>

namespace Coin {

// Represents a signature for a transaction
class TransactionSignature {

public:

    TransactionSignature(const Signature & sig, const SigHashType & type);

    bool operator==(const TransactionSignature & rhs);

    // <max 73b DER signature><1 byte sighash code>
    uchar_vector combinedSignatureAndSighashCode() const;

    // Serialized as scriptSig ready: <OP PUSH next two field><max 73b DER signature><1 byte sighash code>
    uchar_vector opPushForScriptSigSerialized() const;

    // Greatest possible length of a serialized transaction signature
    static uint32_t maxLength();

    // Getters and setters
    Signature sig() const;
    void setSig(const Signature & sig);

    SigHashType type() const;
    void setType(SigHashType type);

private:

    // Signature
    Signature _sig;

    // Sighash type which is basis for signature
    SigHashType _type;

    //unint inputSigned;
};

}

#endif // TRANSACTION_SIGNATURE_HPP

