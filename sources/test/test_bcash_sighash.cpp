#include <string>
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

#include <common/SigHashType.hpp>

#include "json.hpp"

char* sighash_test_vectors_json_file;

using namespace nlohmann; //json header library

TEST(BitcoinCash, SigHashTypeTest)
{
  // Read test vectors from json file - original source
  // https://github.com/Bitcoin-ABC/bitcoin-abc/blob/master/src/test/data/sighash.json
  std::ifstream data(sighash_test_vectors_json_file);
  json vectors_array;
  data >> vectors_array;

  for (json::iterator it = vectors_array.begin(); it != vectors_array.end(); it++) {
    auto vec = *it;

    // skip description elements
    if (vec.size() == 1) continue;

    // ["raw_transaction, script, input_index, hashType, signature_hash (result)"]
    uint32_t hashcode = vec[3];

    // skip non fork-id sighash types
    if(!(hashcode & Coin::HashType::SIGHASH_FORKID)) continue;

    std::string raw_transaction_hex = vec[0];
    std::string subscript_hex = vec[1];
    uint32_t input_index = vec[2];
    std::string signature_hash_hex = vec[4];

    // Make sure hex conversion is not broken
    EXPECT_EQ(raw_transaction_hex, Coin::Transaction(raw_transaction_hex).getSerialized().getHex());
    EXPECT_EQ(subscript_hex, uchar_vector(subscript_hex).getHex());

    auto sighash = Coin::sighash_bitcoin_cash(Coin::Transaction(raw_transaction_hex),
        input_index, uchar_vector(subscript_hex), hashcode, 0);

    EXPECT_EQ(signature_hash_hex, sighash.getReverse().getHex());
  }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    assert(argc == 2);

    sighash_test_vectors_json_file = argv[1];

    return RUN_ALL_TESTS();
}
