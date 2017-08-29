/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/P2PKHScriptSig.hpp>
#include <common/Utilities.hpp> // opPushData

namespace Coin {

P2PKHScriptSig::P2PKHScriptSig(const PublicKey & pk, const TransactionSignature & ts)
    : _pk(pk)
    , _ts(ts) {
}

uint32_t P2PKHScriptSig::maxLength() {
    return P2PKHScriptSig(PublicKey(), TransactionSignature(Signature(), SigHashType())).serialized().size();
}

uchar_vector P2PKHScriptSig::serialized() const {

    uchar_vector serialized;

    // Add signature+sighash type
    serialized += _ts.opPushForScriptSigSerialized();

    // Add public key length
    serialized += opPushData(PublicKey::compressedLength());

    // Add public key
    serialized += _pk.toCompressedRawVector();

    return serialized;

}

P2PKHScriptSig P2PKHScriptSig::deserialize(const uchar_vector & script) {
    uchar_vector rawTxSignatureAndCode;
    uchar_vector rawPublicKey;

    uchar_vector subscript = popData(script, rawTxSignatureAndCode);

    if(!subscript.empty()) {
        popData(subscript, rawPublicKey);
    }

    if(rawTxSignatureAndCode.empty()) {
        throw std::runtime_error("error deserializing p2pkhScriptSig: missing signature.");
    }

    if(rawPublicKey.size() != PublicKey::compressedLength()) {
        throw std::runtime_error("error deserializing p2pkhScriptSig: no valid public key");
    }

    Signature signature = Signature::fromRawDER(uchar_vector(&rawTxSignatureAndCode[0], &rawTxSignatureAndCode[rawTxSignatureAndCode.size()-1]));

    uint32_t hashCode = rawTxSignatureAndCode[rawTxSignatureAndCode.size()-1];

    TransactionSignature txSignature(signature, Coin::SigHashType::fromHashCode(hashCode));

    return P2PKHScriptSig(PublicKey::fromCompressedRaw(rawPublicKey), txSignature);
}

PublicKey P2PKHScriptSig::pk() const {
    return _pk;
}

void P2PKHScriptSig::setPk(const PublicKey & pk) {
    _pk = pk;
}

TransactionSignature P2PKHScriptSig::ts() const {
    return _ts;
}

void P2PKHScriptSig::setTs(const TransactionSignature & ts) {
    _ts = ts;
}

}
