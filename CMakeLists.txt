cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(Common CXX)

option(build_tests "build tests" OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories("${CMAKE_SOURCE_DIR}/include")

set(
  library_sources
    src/DataSizeRepresentation.cpp
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
    src/TimeTools.cpp
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
  file(GLOB tests RELATIVE "${PROJECT_SOURCE_DIR}" "test/test_*.cpp")

  enable_testing()

  foreach(s ${tests})
    get_filename_component (sn ${s} NAME_WE)
    add_executable(${sn} ${s})
    target_link_libraries(${sn} common ${CONAN_LIBS})
    add_test(${sn} "bin/${sn}")
  endforeach(s)

endif()
