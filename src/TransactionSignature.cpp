/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp> // opPushData

namespace Coin {

TransactionSignature::TransactionSignature(const Signature & sig, const SigHashType & type)
    : _sig(sig)
    , _type(type) {
}

bool TransactionSignature::operator==(const TransactionSignature & rhs) {
    return _sig == rhs.sig() && _type == rhs.type();
}

uchar_vector TransactionSignature::combinedSignatureAndSighashCode() const {

    uchar_vector combined;

    combined += _sig.rawDER();
    combined.push_back(_type.hashCode());

    return combined;
}

uchar_vector TransactionSignature::opPushForScriptSigSerialized() const {

    // Serialized script
    uchar_vector serialize;

    // Get combined signature and sighash code
    uchar_vector comb = combinedSignatureAndSighashCode();

    // Add push operation for combined signature and sighash code
    serialize += opPushData(comb.size());

    // Add data
    serialize += comb;

    return serialize;
}

uint32_t TransactionSignature::maxLength() {
    return TransactionSignature(Signature(), SigHashType()).opPushForScriptSigSerialized().size();
}

Signature TransactionSignature::sig() const {
    return _sig;
}

void TransactionSignature::setSig(const Signature & sig) {
    _sig = sig;
}

SigHashType TransactionSignature::type() const {
    return _type;
}

void TransactionSignature::setType(SigHashType type) {
    _type = type;
}

}
