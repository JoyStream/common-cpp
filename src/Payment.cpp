/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#include <common/Payment.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::TxOut

namespace Coin {

Payment::Payment()
    : _value(0) {
}

Payment::Payment(int64_t value, const Coin::P2PKHAddress &addr)
    : _value(value)
    , _serializedScriptPubKey(Coin::P2PKHScriptPubKey(addr.pubKeyHash()).serialize()){

}

Payment::Payment(int64_t value, const Coin::P2SHAddress &addr)
    : _value(value)
    , _serializedScriptPubKey(addr.toP2SHScriptPubKey().serialize()){
}

Payment::Payment(int64_t value, const Coin::PubKeyHash &pkHash)
    : _value(value)
    , _serializedScriptPubKey(Coin::P2PKHScriptPubKey(pkHash).serialize()){
}

Coin::TxOut Payment::txOut() const {
    return Coin::TxOut(_value, _serializedScriptPubKey);
}

int64_t Payment::value() const {
    return _value;
}

void Payment::setValue(int64_t value) {
    _value = value;
}

uchar_vector Payment::serializedScriptPubKey() const {
    return _serializedScriptPubKey;
}

void Payment::setSerializedScriptPubKey(const uchar_vector &scriptPubKey) {
    _serializedScriptPubKey = scriptPubKey;
}

}
