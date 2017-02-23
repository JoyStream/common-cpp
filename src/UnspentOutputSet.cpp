/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#include <common/UnspentOutputSet.hpp>

#include <CoinCore/CoinNodeData.h>

namespace Coin {

UnspentOutputSet::UnspentOutputSet()
{

}

UnspentOutputSet::UnspentOutputSet(const std::set<std::shared_ptr<UnspentOutput> > & utxos) {
    for(const auto &output : utxos)
        insert(output);
}

uint64_t UnspentOutputSet::value() const {
    using namespace std;

    uint64_t total = 0;

    for(auto & utxo : *this) {
        total += utxo->value();
    }

    return total;
}

void UnspentOutputSet::finance(Transaction & cointx, const SigHashType & sigHashType) const {

    // If any Utxo has already been spent in the transaction throw an error
    for(auto & input : cointx.inputs) {
        // Find associated utxo
        auto utxo = outputFromOutPoint(input.previousOut);
        if(utxo) {
            throw std::runtime_error("The transaction is already financed by a utxo in the set");
        }
    }

    // To allow for ability to finance a transaction from multiple sets, we will not clear the inputs of the transaction.
    // For transactions which will be signed without ANYONE_CAN_PAY
    // the transaction is expected to have no inputs

    if(!sigHashType.anyOneCanPay()){
        // To avoid misuse when chaining multiple finance calls..
        if(cointx.inputs.size() > 0 ) {
            throw std::runtime_error("Only ANYONE_CAN_PAY sighash types can have non zero inputs size");
        }

        // Ensure this set has enough value to finance the transaction
        uint64_t totalOutputValue = 0;

        for(const Coin::TxOut & output : cointx.outputs) {
            totalOutputValue += output.value;
        }

        if(value() < totalOutputValue) {
            throw std::runtime_error("UnspentOutpustSet value insufficient to finance transaction");
        }
    }

    // Add new inputs to transaction
    for(auto & utxo : *this) {
        cointx.inputs.push_back(utxo->unsignedSpendingInput());
    }

    // Sign the inputs
    for(auto & input : cointx.inputs) {
        // Find associated utxo
        auto utxo = outputFromOutPoint(input.previousOut);
        if(utxo) {
            input.scriptSig = utxo->scriptSig(cointx, sigHashType);
        }
    }
}

std::shared_ptr<UnspentOutput> UnspentOutputSet::outputFromOutPoint(const Coin::OutPoint & outpoint) const {

    for(auto & utxo : *this) {
        if(utxo->outPoint() == typesafeOutPoint(outpoint)) return utxo;
    }

    return nullptr;
}

}
