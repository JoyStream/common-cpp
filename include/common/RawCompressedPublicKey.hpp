#ifndef RAW_COMPRESSED_PUBLIC_KEY_HPP
#define RAW_COMPRESSED_PUBLIC_KEY_HPP

#include <common/FixedUCharArray.hpp>

#define RAW_COMPRESSED_PUBLIC_KEY_BYTE_LENGTH 33

namespace Coin {

typedef FixedUCharArray<RAW_COMPRESSED_PUBLIC_KEY_BYTE_LENGTH> RawCompressedPublicKey;

}

#endif // RAW_COMPRESSED_PUBLIC_KEY_HPP
