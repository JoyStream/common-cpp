#include <common/P2PKScriptHashOutput.hpp>
#include <common/P2PKScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp>

namespace Coin {

P2PKScriptHashOutput::P2PKScriptHashOutput()
{

}

P2PKScriptHashOutput::P2PKScriptHashOutput(const KeyPair &keypair, const typesafeOutPoint &outpoint, uint64_t setvalue, uchar_vector scriptpubkey, uchar_vector redeemscript)
    : UnspentOutput(keypair, outpoint, setvalue, scriptpubkey, redeemscript)
{
    Coin::P2PKScriptPubKey script(keyPair().pk());

    if(redeemScript() != script.serialize()) {
        throw std::runtime_error("output doesn't use a p2pk redeem script");
    }
}

uchar_vector P2PKScriptHashOutput::getScriptSig(const TransactionSignature &sig) const {
    uchar_vector scriptSig;

    scriptSig += sig.opPushForScriptSigSerialized();
    scriptSig += opPushData(redeemScript().size());
    scriptSig += redeemScript();

    return scriptSig;
}

}
