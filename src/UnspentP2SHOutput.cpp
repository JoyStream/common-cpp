/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/UnspentP2SHOutput.hpp>
#include <CoinCore/CoinNodeData.h>
#include <common/Utilities.hpp>

namespace Coin {

UnspentP2SHOutput::UnspentP2SHOutput()
    : UnspentOutput() {
}

UnspentP2SHOutput::UnspentP2SHOutput(const KeyPair & keyPair, const uchar_vector & redeemScript, const uchar_vector & optionalData, const typesafeOutPoint & outPoint, uint64_t setValue)
    : UnspentOutput(outPoint, setValue)
    , _keyPair(keyPair)
    , _redeemScript(redeemScript)
    , _optionalDataLoadingScriptChunk(optionalData) {
}

bool UnspentP2SHOutput::operator==(const UnspentP2SHOutput & o) const {

    return _keyPair == o.keyPair() &&
           _redeemScript == o.redeemScript() &&
           _optionalDataLoadingScriptChunk == o.optionalData() &&
           outPoint() == o.outPoint() &&
           value() == o.value();
}

bool UnspentP2SHOutput::operator!=(const UnspentP2SHOutput & o) const {
    return !(*this == o);
}

KeyPair UnspentP2SHOutput::keyPair() const {
    return _keyPair;
}

void UnspentP2SHOutput::setKeyPair(const KeyPair &keyPair) {
    _keyPair = keyPair;
}

uchar_vector UnspentP2SHOutput::redeemScript() const {
    return _redeemScript;
}

void UnspentP2SHOutput::setRedeemScript(const uchar_vector & script) {
    _redeemScript = script;
}

uchar_vector UnspentP2SHOutput::optionalData() const {
    return _optionalDataLoadingScriptChunk;
}

void UnspentP2SHOutput::setOptionalData(const uchar_vector & optionalData) {
    _optionalDataLoadingScriptChunk = optionalData;
}

uchar_vector UnspentP2SHOutput::scriptPubKey() const {
    return P2SHScriptPubKey(RedeemScriptHash::fromRawScript(redeemScript())).serialize();
}

uchar_vector UnspentP2SHOutput::sighash(const Transaction & tx, const SigHashType &sigHashType) const {
    return ::Coin::sighash(tx, outPoint(), redeemScript(), sigHashType);
}

TransactionSignature UnspentP2SHOutput::transactionSignature(const Transaction & tx, const SigHashType &sigHashType) const {
    uchar_vector sigHash = sighash(tx, sigHashType);
    return TransactionSignature(keyPair().sk().sign(sigHash), sigHashType);
}

uchar_vector UnspentP2SHOutput::scriptSig(const Transaction & tx, const SigHashType &sigHashType) const {

    uchar_vector scriptSig;

    scriptSig += transactionSignature(tx, sigHashType).opPushForScriptSigSerialized();

    // Optional Data (without a size opcode, so it can contain multiple chunks)
    if(optionalData().size() > 0)
        scriptSig += optionalData();

    scriptSig += opPushData(redeemScript().size());
    scriptSig += redeemScript();

    return scriptSig;
}

}
