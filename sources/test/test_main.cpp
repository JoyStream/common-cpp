#include <string>
#include <gtest/gtest.h>

#include <CoinCore/bip39.h>
#include <CoinCore/secp256k1_openssl.h>
#include <CoinCore/hdkeys.h>
#include <CoinCore/Base58Check.h>

#include <common/TransactionId.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/Signature.hpp>
#include <common/SigHashType.hpp>
#include <common/TransactionSignature.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/MultisigScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/P2SHScriptSig.hpp>
#include <common/Utilities.hpp>
#include <common/Seed.hpp>
#include <common/Entropy.hpp>
#include <common/RelativeLockTime.hpp>

typedef unsigned char uchar;

#define TEST_PRIVATE_KEY1 "0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20"
#define TEST_PRIVATE_KEY2 "0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1faa"

TEST(commonTest, UCharArrayTest)
{
    // Zeroed array with default constructor
    {
        Coin::UCharArray<16> array;

        for (unsigned int i = 0; i < 16; i++) {
            EXPECT_EQ(array.at(i), 0x00);
        }
    }

    // Set Raw Bytes from unsigned char*
    {
        Coin::UCharArray<16> array(uchar_vector(16, 'A')); //16 0x41 bytes

        for (unsigned int i = 0; i < 16; i++) {
            EXPECT_EQ(array.at(i), 0x41);
        }
    }

    // Set Raw Bytes from uchar_vector
    {
        Coin::UCharArray<16> array(uchar_vector("41414141414141414141414141414141")); //16 0x41 bytes

        for (unsigned int i = 0; i < 16; i++) {
            EXPECT_EQ(array.at(i), 0x41);
        }
    }

    // Set Raw Bytes
    {
        Coin::UCharArray<16> array(uchar_vector(16, 'A')); //16 0x41 bytes

        for (unsigned int i = 0; i < 16; i++) {
            EXPECT_EQ(array.at(i), 0x41);
        }
    }

    // Set Hex
    {
        Coin::UCharArray<32> array("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");

        for (unsigned int i = 0; i < 32; i++) {
            EXPECT_EQ(array.at(i), i);
        }
    }

    // Catch invalid hex characters
    {
        try {
            Coin::UCharArray<1> array("0@"); //invalid hex character
            FAIL();
        } catch(std::exception &e) {
            ASSERT_STREQ("not a hex digit", e.what());
        }
    }

    // Throws error with invalid sizes
    {
        try {
            Coin::UCharArray<1> array1(uchar_vector(2)); //too long
            FAIL();
        } catch(std::exception &e) { }

        try {
            Coin::UCharArray<2> array2(uchar_vector(1)); //too short
            FAIL();
        } catch(std::exception &e) { }

        try {
            Coin::UCharArray<1> array3("0000"); //too long
            FAIL();
        } catch(std::exception &e) { }

        try {
            Coin::UCharArray<2> array4("00"); //too short
            FAIL();
        } catch(std::exception &e) { }
    }

    // hex string must be even length
    {
        EXPECT_THROW(new Coin::UCharArray<3>("000"), std::runtime_error);
    }

    // toHex
    {
        std::string str = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f";
        Coin::UCharArray<32> array(str);
        EXPECT_EQ(str, array.getRawHex());
    }

    // fromHex - allows capital and lowercase letters
    {
        std::string str1 = "ABCDEF";
        Coin::UCharArray<3> array1(str1);

        std::string str2 = "abcdef";
        Coin::UCharArray<3> array2(str2);

        EXPECT_EQ(array1, array2);
    }
}

TEST(commonTest, transactionId)
{
    Coin::Transaction tx("01000000010000000000000000000000000000000000000000000000000000000000000000000000001774657374373230373734333134313533393132383336390000000001807c814a00000000232102a44f60c94b840854db8c673e280dbc76b2975c6cf10e351ef6208f7f546e2130ac00000000");
    std::string txId("6550109a0be63c8ae647478be1320fa00ff9d19d509f18432723e160a694949d");

    Coin::TransactionId id = Coin::TransactionId::fromRPCByteOrder(txId);
    Coin::TransactionId derivedId = Coin::TransactionId::fromTx(tx);

    EXPECT_EQ(derivedId, id);
    EXPECT_EQ(txId, id.toRPCByteOrder());
}

TEST(commonTest, basic)
{
    // Create fresh private keys
    CoinCrypto::secp256k1_key key;
    key.newKey();

    // Message to be signed
    uchar_vector message(100, 55);

    // Compute signature
    bytes_t sig = CoinCrypto::secp256k1_sign(key, message);

    // Create signature checking key for this public key
    CoinCrypto::secp256k1_key signatureCheckingKey;
    signatureCheckingKey.setPubKey(key.getPubKey());

    // Check valid signature
    EXPECT_TRUE(CoinCrypto::secp256k1_verify(signatureCheckingKey, message, sig, CoinCrypto::SignatureFlag::SIGNATURE_ENFORCE_LOW_S));

    // Check false signature
    try {
        // Should throw
        bool hold = CoinCrypto::secp256k1_verify(signatureCheckingKey, message, bytes_t(sig.size()), CoinCrypto::SignatureFlag::SIGNATURE_ENFORCE_LOW_S);
        EXPECT_TRUE(!hold);
    } catch (std::runtime_error & e) {
        EXPECT_TRUE(true);
    }

    // Check correct signature for incorrect message
    try {
        // Should throw or be invalid
        bool holds  = CoinCrypto::secp256k1_verify(signatureCheckingKey, uchar_vector(23, 0), sig, CoinCrypto::SignatureFlag::SIGNATURE_ENFORCE_LOW_S);
        EXPECT_TRUE(!holds);
    } catch (const std::runtime_error & e) {
        EXPECT_TRUE(true);
    }
}

TEST(commonTest, makeAndCheckSig)
{
    // Generate a new private key
    Coin::PrivateKey sk = Coin::PrivateKey::fromRawHex(TEST_PRIVATE_KEY1);

    // Get corresponding public key
    Coin::PublicKey pk = sk.toPublicKey();

    // Message to be signed
    uchar_vector message(100, 55);

    // Sign message
    Coin::Signature sig = sk.sign(message);

    // Check that valid signature works
    // Should never throw, since its valid
    EXPECT_TRUE(pk.verify(message, sig));

    // Check that false (blank) signature does not work
    try {
        // may throw
        bool valid = pk.verify(message, Coin::Signature());
        EXPECT_TRUE(!valid);
    } catch (const std::runtime_error & e) {
        EXPECT_TRUE(true);
    }

    // Check that valid signature does not work with incorrect public key
    try {
        Coin::PrivateKey skNew = Coin::PrivateKey::fromRawHex(TEST_PRIVATE_KEY2);
        bool valid = skNew.toPublicKey().verify(message, sig);
        EXPECT_TRUE(!valid);
    } catch (const std::runtime_error & e) {
        EXPECT_TRUE(true);
    }
}

TEST(commonTest, P2PKHScriptPubKey)
{
    /**
     * Bitcore script test data generator
     *   var compressedPK = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);
     *   var scriptPubkey = bitcore.Script.buildPublicKeyHashOut(compressedPK);
     *   console.log(scriptPubkey);
     *   console.log(scriptPubkey.toBuffer());
     *
     *   Output:
     *   <Script: OP_DUP OP_HASH160 20 0xcbf730e06e5f8e4fc44f071d436a4660ddde3e47 OP_EQUALVERIFY OP_CHECKSIG>
     *   <Buffer 76 a9 14 cb f7 30 e0 6e 5f 8e 4f c4 4f 07 1d 43 6a 46 60 dd de 3e 47 88 ac>
     */

    Coin::PublicKey pk = Coin::PublicKey::fromCompressedRawHex("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector serializedP2PKH("76a914cbf730e06e5f8e4fc44f071d436a4660ddde3e4788ac");

    Coin::P2PKHScriptPubKey script(pk);

    EXPECT_EQ(serializedP2PKH, script.serialize());
}

TEST(commonTest, P2PKHScriptSig)
{
    /**
     *    / / P2PKHScriptSig                                                    *
     *    // NOTICE: PUBLIC KEY AND SIGNATURES DO NOT MATCH, ITS JUST FOR TESTING
     *    var compressedPK = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);
     *
     *    var sigHexaStr = '1cd5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec12fc1188e8b' +
     *    '0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6';
     *    var sig = Signature.fromCompact(new Buffer(sigHexaStr, 'hex'));
     *
     *    console.log(sig.toBuffer().toString('hex'));
     *
     *    var scriptSig = Script.buildPublicKeyHashIn(compressedPK, sig, bitcore.crypto.Signature.SIGHASH_ALL);
     *    console.log(scriptSig.toString());
     *    console.log(scriptSig.toBuffer().toString('hex'));
     *
     *    Output:
     *    3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6
     *    72 0x3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601 33 0x030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc
     *    483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc60121030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc
     */

    uchar_vector rawPk("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector rawSig("3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6");
    Coin::PublicKey pk = Coin::PublicKey::fromCompressedRaw(rawPk);
    Coin::TransactionSignature ts(Coin::Signature::fromRawDER(rawSig), Coin::SigHashType::standard());
    Coin::P2PKHScriptSig sig(pk, ts);
    uchar_vector serializedP2PKHScriptSig("483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc60121030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");

    EXPECT_EQ(serializedP2PKHScriptSig, sig.serialized());

    Coin::P2PKHScriptSig deserializedScript(Coin::P2PKHScriptSig::deserialize(serializedP2PKHScriptSig));

    EXPECT_EQ(pk, deserializedScript.pk());
    //EXPECT_EQ(ts, deserializedScript.ts());

    EXPECT_EQ(serializedP2PKHScriptSig, deserializedScript.serialized());
}

TEST(commonTest, MultiSigScriptPubKey)
{
    /**
     *    / / MultisigScriptPubKey                                         *
     *    var compressedPK_1 = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);
     *    var compressedPK_2 = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc', net);
     *
     *    var scriptPubKey = Script.buildMultisigOut([compressedPK_1, compressedPK_2], 2, {noSorting : true}); // remembe to not sort
     *    var redeemScriptHash = bitcore.crypto.Hash.sha256ripemd160(scriptPubKey.toBuffer());
     *
     *    console.log(scriptPubKey.toString());
     *    console.log(scriptPubKey.toBuffer().toString('hex'));
     *    console.log(redeemScriptHash.toString('hex'));
     *
     *    Output:
     *    OP_2 33 0x030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc 33 0x030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc OP_2 OP_CHECKMULTISIG
     *    5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc52ae
     *    4fbd6060861fed97ef2b95e6afff4afb2943cc15
     */

    uchar_vector rawPk_1("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector rawPk_2("030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc");
    uchar_vector rawScript("5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc52ae");
    //uchar_vector rawScriptHash("4fbd6060861fed97ef2b95e6afff4afb2943cc15");

    Coin::PublicKey pk1 = Coin::PublicKey::fromCompressedRaw(rawPk_1);
    Coin::PublicKey pk2 = Coin::PublicKey::fromCompressedRaw(rawPk_2);

    Coin::MultisigScriptPubKey script(std::vector<Coin::PublicKey>({pk1, pk2}), 2);

    EXPECT_EQ(rawScript, script.serialized());
    //QVERIFY(script.scriptHash().toUCharVector() == rawScriptHash);
}

TEST(commonTest, P2SHScriptPubKey)
{
    /*
     *   // P2SHScriptPubKey
     *   var redeemScript = bitcore.Script("5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc52ae");
     *   var scriptPubKey = bitcore.Script.buildScriptHashOut(redeemScript);
     *
     *   console.log(redeemScript.toString())
     *   console.log(scriptPubKey.toString());
     *   console.log(scriptPubKey.toBuffer().toString('hex'));
     *
     *   Output:
     *   OP_2 33 0x030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc 33 0x030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc OP_2 OP_CHECKMULTISIG
     *   OP_HASH160 20 0x4fbd6060861fed97ef2b95e6afff4afb2943cc15 OP_EQUAL
     *   a9144fbd6060861fed97ef2b95e6afff4afb2943cc1587
     */

    uchar_vector rawRedeemScript("5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc52ae");
    uchar_vector rawScript("a9144fbd6060861fed97ef2b95e6afff4afb2943cc1587");

    Coin::P2SHScriptPubKey script = Coin::P2SHScriptPubKey(Coin::RedeemScriptHash::fromRawScript(rawRedeemScript));

    EXPECT_EQ(rawScript, script.serialize());
}

TEST(commonTest, P2SHMultisigScriptSig)
{
    /**
     *   // MultisigScriptSig
     *   // NOTICE: PUBLIC KEY AND SIGNATURES DO NOT MATCH, ITS JUST FOR TESTING
     *   var compressedPK_1 = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);
     *   var compressedPK_2 = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);
     *
     *   var sigHexaStr_1 = '1cd5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec12fc1188e8b' +
     *       '0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6';
     *   var sig_1 = Signature.fromCompact(new Buffer(sigHexaStr_1, 'hex'));
     *
     *   // buildP2SHMultisigIn needs sighash byte added to each signature already
     *   var sig_1_buffer = bitcore.util.buffer.concat([
     *       sig_1.toDER(),
     *       bitcore.util.buffer.integerAsSingleByteBuffer(bitcore.crypto.Signature.SIGHASH_ALL)
     *   ]);
     *
     *
     *   var sigHexaStr_2 = '1cd5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec12fc1188e8b' +
     *       '0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6';
     *   var sig_2 = Signature.fromCompact(new Buffer(sigHexaStr_2, 'hex'));
     *   var sig_2_buffer = bitcore.util.buffer.concat([
     *       sig_2.toDER(),
     *       bitcore.util.buffer.integerAsSingleByteBuffer(bitcore.crypto.Signature.SIGHASH_ALL)
     *   ]);
     *
     *   var scriptSig = Script.buildP2SHMultisigIn([compressedPK_1, compressedPK_2], 2, [sig_1_buffer, sig_2_buffer], {noSorting : true});
     *
     *   console.log(sig_1.toString());
     *   console.log(sig_2.toString());
     *   console.log(scriptSig.toString());
     *   console.log(scriptSig.toBuffer().toString('hex'));
     *
     *   Output:
     *   3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6
     *   3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6
     *   OP_0 72 0x3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601 72 0x3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601 71 0x5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc52ae
     *   00483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601475221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc52ae
     */

    uchar_vector rawPk_1("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector rawPk_2("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector rawSig_1("3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6");
    uchar_vector rawSig_2("3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6");
    uchar_vector script("00483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601475221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc52ae");

    Coin::TransactionSignature ts_1(Coin::Signature::fromRawDER(rawSig_1), Coin::SigHashType::standard());
    Coin::TransactionSignature ts_2(Coin::Signature::fromRawDER(rawSig_2), Coin::SigHashType::standard());

    Coin::PublicKey pk_1 = Coin::PublicKey::fromCompressedRaw(rawPk_1);
    Coin::PublicKey pk_2 = Coin::PublicKey::fromCompressedRaw(rawPk_2);

    Coin::MultisigScriptPubKey redeemScript(std::vector<Coin::PublicKey>{pk_1, pk_2}, 2);
    Coin::P2SHScriptSig sig(std::vector<Coin::TransactionSignature>{ts_1, ts_2}, redeemScript.serialized());

    uchar_vector ser = sig.serialized();
    //qDebug() << QString::fromStdString(ser.getHex());
    EXPECT_EQ(script, sig.serialized());
}

TEST(commonTest, BIP39)
{
    // Test Vectors from
    // https://github.com/trezor/python-mnemonic/blob/master/vectors.json
    {
        Coin::Entropy entropy = Coin::Entropy::fromRawHex("00000000000000000000000000000000");
        EXPECT_STREQ(entropy.getRawHex().c_str(), "00000000000000000000000000000000");
        EXPECT_STREQ(entropy.mnemonic().c_str(), "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about");
        EXPECT_STREQ(Coin::Entropy::fromMnemonic("abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about").getRawHex().c_str(), "00000000000000000000000000000000");
        EXPECT_STREQ(entropy.seed("TREZOR").getRawHex().c_str(),  "c55257c360c07c72029aebc1b53c05ed0362ada38ead3e3e9efa3708e53495531f09a6987599d18264c1e1c92f2cf141630c7a3c4ab7c81b2f001698e7463b04");
    }
    {
        Coin::Entropy entropy = Coin::Entropy::fromRawHex("7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f");
        EXPECT_STREQ(entropy.getRawHex().c_str(), "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f");
        EXPECT_STREQ(entropy.mnemonic().c_str(), "legal winner thank year wave sausage worth useful legal winner thank yellow");
        EXPECT_STREQ(entropy.seed("TREZOR").getRawHex().c_str(),  "2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6fa457fe1296106559a3c80937a1c1069be3a3a5bd381ee6260e8d9739fce1f607");
    }
    {
        Coin::Entropy entropy = Coin::Entropy::fromRawHex("80808080808080808080808080808080");
        EXPECT_STREQ(entropy.getRawHex().c_str(), "80808080808080808080808080808080");
        EXPECT_STREQ(entropy.mnemonic().c_str(), "letter advice cage absurd amount doctor acoustic avoid letter advice cage above");
        EXPECT_STREQ(entropy.seed("TREZOR").getRawHex().c_str(), "d71de856f81a8acc65e6fc851a38d4d7ec216fd0796d0a6827a3ad6ed5511a30fa280f12eb2e47ed2ac03b5c462a0358d18d69fe4f985ec81778c1b370b652a8");
    }

}

TEST(commonTest, TimeRelativeLockTimeEncoding)
{
    uint16_t counter = 0xbeef;
    Coin::RelativeLockTime relativeLockTime = Coin::RelativeLockTime::fromTimeUnits(counter);
    uchar_vector data = relativeLockTime.toScriptData();

    EXPECT_EQ(4, (int)data.size());

    // sign bit unset,
    // most significant bit (after the sign bit) should be set
    // so OP_CHECKSEQUENCEVERIFY will interpret the the relative locktime
    // as time
    EXPECT_EQ(0x40, data.at(3) & 0xc0);

    // 16-bits representing locktime value
    EXPECT_EQ(uchar(0xbe), data.at(2));
    EXPECT_EQ(uchar(0xef), data.at(1));

    EXPECT_EQ(uint32_t(0x0040beef), relativeLockTime.toSequenceNumber());
}

TEST(commonTest, BlockRelativeLockTimeEncoding)
{
    uint16_t counter = 0xbeef;
    Coin::RelativeLockTime relativeLockTime = Coin::RelativeLockTime::fromBlockUnits(counter);

    EXPECT_EQ(Coin::RelativeLockTime::Units::Blocks, relativeLockTime.units());

    uchar_vector data = relativeLockTime.toScriptData();

    EXPECT_EQ(4, (int)data.size());

    // sign bit unset and
    // most significant bit (after the sign bit) should be unset
    // so OP_CHECKSEQUENCEVERIFY will interpret the the relative locktime
    // as blocks
    EXPECT_EQ(0x00, data.at(3) & 0xc0);

    // 16-bits representing locktime value
    EXPECT_EQ(uchar(0xbe), data.at(2));
    EXPECT_EQ(uchar(0xef), data.at(1));

    EXPECT_EQ(uint32_t(0x0000beef), relativeLockTime.toSequenceNumber());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
