#include <common/Entropy.hpp>
#include <common/Seed.hpp>
#include <CoinCore/hdkeys.h>
#include <CoinCore/Base58Check.h>

#include <iostream>

struct BIP44 {
  static const uint32_t PURPOSE = 0x8000002C;
  static const uint32_t DEFAULT_ACCOUNT = 0x80000000;
  static const uint32_t COIN_TYPE_BITCOIN = 0x80000000;
};

int main(int argc, char* argv[])
{
  Coin::Entropy entropy;

  std::string wordlist;
  std::cout << "Enter seed word list then press enter:" << std::endl;

  // Read the wordlist from stdin
  getline(std::cin, wordlist);

  entropy = Coin::Entropy::fromMnemonic(wordlist);

  // BIP39 Seed
  auto seed = entropy.seed();

  // BIP44 Wallet - m/44'/0'/0'
  auto accountPrivKeychain = seed.generateHDKeychain().getChild(BIP44::PURPOSE).getChild(BIP44::COIN_TYPE_BITCOIN).getChild(BIP44::DEFAULT_ACCOUNT);
  auto accountPubKeychain = accountPrivKeychain.getPublic();

  // Extened public key (xpub)
  auto xpub = accountPubKeychain.extkey();

  std::cout << toBase58Check(xpub) << std::endl;

  return 0;
}
