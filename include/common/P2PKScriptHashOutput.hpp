#ifndef COIN_P2PK_SCRIPT_HASH_OUTPUT_H
#define COIN_P2PK_SCRIPT_HASH_OUTPUT_H

#include <common/UnspentOutput.hpp>

namespace Coin {

class P2PKScriptHashOutput : public UnspentOutput
{
public:
    P2PKScriptHashOutput();

    // Instance from base output type
    P2PKScriptHashOutput(const KeyPair & keypair, const typesafeOutPoint & outpoint, uint64_t setvalue, uchar_vector scriptpubkey, uchar_vector redeemscript);

    // P2PK redeem scripts are immediately spendable
    bool spendable() const { return true; }

    uchar_vector getScriptSig(const TransactionSignature &sig) const;
};

}
#endif // COIN_P2PK_SCRIPT_HASH_OUTPUT_H
