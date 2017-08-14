#include <iostream>
#include <CoinCore/CoinNodeData.h>
#include <stdutils/uchar_vector.h>
#include <common/PrivateKey.hpp>
#include <common/Seed.hpp>
#include <common/Entropy.hpp>

int main() {
    uchar_vector v;

    Coin::Transaction tx;

    Coin::PrivateKey sk = Coin::PrivateKey::generate();

    Coin::Entropy e = Coin::Entropy::generate();

    Coin::Seed seed = e.seed();

    return 0;
}
