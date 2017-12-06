cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(Common CXX)

option(build_tests "build tests" OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories("${CMAKE_SOURCE_DIR}/include")

set(
  library_sources
    src/PublicKey.cpp
    src/PrivateKey.cpp
    src/Base58CheckEncodable.cpp
    src/Signature.cpp
    src/KeyPair.cpp
    src/P2PKHAddress.cpp
    src/UnspentOutput.cpp
    src/UnspentP2SHOutput.cpp
    src/UnspentP2PKHOutput.cpp
    src/Utilities.cpp
    src/typesafeOutPoint.cpp
    src/P2SHAddress.cpp
    src/Seed.cpp
    src/Entropy.cpp
    src/Network.cpp
    src/SigHashType.cpp
    src/MultisigScriptPubKey.cpp
    src/P2SHScriptPubKey.cpp
    src/MultisigScriptSig.cpp
    src/TransactionSignature.cpp
    src/P2SHScriptSig.cpp
    src/P2PKHScriptPubKey.cpp
    src/P2PKHScriptSig.cpp
    src/P2PKScriptPubKey.cpp
    src/Payment.cpp
    src/TransactionId.cpp
    src/UnspentOutputSet.cpp
    src/RedeemScriptHash.cpp
    src/RelativeLockTime.cpp
)

# === build library ===
add_library(common ${library_sources})

# === build tests ===
if(build_tests)
  enable_testing()

  # Main unit tests
  add_executable(test_main "${CMAKE_CURRENT_LIST_DIR}/test/test_main.cpp")
  target_link_libraries(test_main common ${CONAN_LIBS})
  add_test(MainTests "bin/test_main")

  # sighash unit tests
  add_executable(test_sighash "${CMAKE_CURRENT_LIST_DIR}/test/test_sighash.cpp")
  target_link_libraries(test_sighash common ${CONAN_LIBS})
  add_test(SigHash_Tests "bin/test_sighash" "${CMAKE_CURRENT_LIST_DIR}/test/data/sighash.json")

endif()
