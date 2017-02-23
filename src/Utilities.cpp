/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <cassert>

#include <common/Utilities.hpp>

#include <stdutils/uchar_vector.h>
#include <common/Network.hpp>
#include <common/SigHashType.hpp>
#include <common/TransactionSignature.hpp>
#include <common/PrivateKey.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/typesafeOutPoint.hpp>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <cassert>

namespace Coin {

    std::string toHex(const std::vector<unsigned char> &input) {
        return toHex(input.data(), input.size());
    }

    std::string toHex(const unsigned char * input, size_t length) {
        static const char* const lut = "0123456789abcdef";

        std::string output;

        output.reserve(2 * length);

        for (size_t i = 0; i < length; ++i)
        {
            const unsigned char c = input[i];
            output.push_back(lut[c >> 4]);
            output.push_back(lut[c & 15]);
        }

        return output;
    }

    std::vector<unsigned char> fromHex(const std::string& hex) {
        // the order of the characters must match their order as per their ascii value
        // for std::lower_bound to work as expected
        static const char* hex_chars_begin = "0123456789abcdef";

        static const char* hex_chars_end = hex_chars_begin + strlen(hex_chars_begin);

        assert(strlen(hex_chars_begin) == 16);

        size_t length = hex.length();

        // Hex string must already be 0 padded - only even length string allowed
        if (length % 2 == 1) throw std::runtime_error("odd length");

        // make a copy to convert to lowercase
        std::string input = hex;

        // convert to lowercase to support both cases
        std::transform(input.begin(), input.end(), input.begin(), tolower);

        // byte array to store result
        std::vector<unsigned char> output;

        // process two hex digits at a time
        for (size_t i = 0; i < length; i += 2)
        {
            char c;

            // verify first hex digit (4 most significant bits in byte)
            // is in valid range of characters
            c = input[i];
            const char* n1 = std::lower_bound(hex_chars_begin, hex_chars_end, c);

            // if c was not found in the hex chars range, the last value in the array is returned
            // 'f' so if the character at n1 is not c the character was not found
            if (*n1 != c) throw std::runtime_error("not a hex digit");

            // verify second hex digit (4 least significant bits)
            // is in valid range of characters
            c = input[i + 1];
            const char* n2 = std::lower_bound(hex_chars_begin, hex_chars_end, c);
            if (*n2 != c) throw std::runtime_error("not a hex digit");

            // convert both pointers to decimal value (position in
            output.push_back(((n1 - hex_chars_begin) << 4) | (n2 - hex_chars_begin));
        }

        assert(output.size() * 2 == length);

        return output;
    }

    const unsigned char * networkToAddressVersions(Network network) {

        switch(network) {
            case Network::testnet3: return testnet3AddressVersions;
            case Network::mainnet: return mainnetAddressVersions;
            case Network::regtest: return testnet3AddressVersions;
            default:
                    assert(false);
        }
    }

    uchar_vector opPushData(uint32_t nBytes)
    {
        uchar_vector rval;
        // push data upto 75 bytes long
        if (nBytes <= 0x4b) {
            rval.push_back((unsigned char)nBytes);
        }
        // OP_PUSHDATA1
        else if (nBytes <= 0xff) {
            rval.push_back(0x4c);
            rval.push_back((unsigned char)nBytes);
        }
        // OP_PUSHDATA2
        else if (nBytes <= 0xffff) {
            rval.push_back(0x4d);
            rval.push_back((unsigned char)(nBytes & 0xff));
            rval.push_back((unsigned char)(nBytes >> 8));
        }
        // OP_PUSHDATA4
        else {
            rval.push_back(0x4e);
            rval.push_back((unsigned char)(nBytes & 0xff));
            rval.push_back((unsigned char)((nBytes >> 8) & 0xff));
            rval.push_back((unsigned char)((nBytes >> 16) & 0xff));
            rval.push_back((unsigned char)(nBytes >> 24));
        }

        return rval;
    }

    uchar_vector opPushNumber(uint32_t value) {
        if(value == 0) {
            return uchar_vector(1, 0x00);
        }

        if(value < 17) {
            return uchar_vector(1, 0x51 + (unsigned char)value - 1);
        }

        uchar_vector encodedNumber = serializeScriptNum(value);
        uchar_vector data;
        data += opPushData(encodedNumber.size());
        data += encodedNumber;
        return data;
    }

    /*
     * This method extracts the next data item from the script and assigns it to poppedData
     * and returns a subscript following the end of the data.
     *
     * If the first byte (operation) is not a push data operation or
     * script is too short to contain the expected data the popped data and returned subscript
     * will be an empty uchar_vector
     */
    uchar_vector popData(const uchar_vector & script, uchar_vector & poppedData)
    {
        uchar_vector subscript;
        poppedData = uchar_vector();

        if(script.empty()) return uchar_vector();

        uint32_t dataSize = 0;
        uint32_t offset = 0;

        // OP_0 / OP_FALSE
        if (script[0] == 0x00) {
            offset = 1;
            poppedData = uchar_vector(1, 0x00);
        }
        // push data upto 75 bytes long
        else if (script[0] <= 0x4b && script.size() > 1) {
            dataSize = script[0];
            offset = 1;
        }
        // OP_PUSHDATA1
        else if (script[0] == 0x4c && script.size() > 1) {
            dataSize = script[1];
            offset = 1;
        }
        // OP_PUSHDATA2
        else if (script[0] == 0x4d && script.size() > 2) {
            dataSize = script[1];
            dataSize += (script[2] << 8);
            offset = 3;
        }
        // OP_PUSHDATA4
        else if(script[0] == 0x4e && script.size() > 3){
            dataSize = script[1];
            dataSize += ((uint32_t)script[2]) << 8;
            dataSize += ((uint32_t)script[3]) << 16;
            dataSize += ((uint32_t)script[4]) << 24;
            offset = 5;
        }
        // OP_1 ... OP_16
        else if(script[0] >= 0x51 || script[0] <= 0x60) {
            offset = 1;
            poppedData = uchar_vector(1, script[0] - 0x51 + 1);
        }else{
            // operation is not a push data op, or not enough data to pop
        }

        if(script.size() >= (offset + dataSize)){

            if(script.size() > (offset + dataSize)){
                subscript = uchar_vector(script.begin() + offset + dataSize, script.end());
            }

            if(dataSize > 0) {
                poppedData = uchar_vector(script.begin() + offset, script.begin() + offset + dataSize);
            }
        }

        return subscript;
    }

    /**
    unsigned int extendedPrivateKeyVersionBytes(Network network) {

        switch(network) {

            case Network::testnet3: return 0x04358394;
            case Network::mainnet: return 0x0488ADE4;
            default:
                Q_ASSERT(false);
        }
    }

    unsigned int extendedPublicKeyVersionBytes(Network network) {

        switch(network) {

            case Network::testnet3: return 0x043587CF;
            case Network::mainnet: return 0x0488B21E;
            default:
                Q_ASSERT(false);
        }
    }
    */

    uchar_vector sighash(const Transaction &tx,
                         uint inputIndex,
                         const uchar_vector &subscript,
                         const SigHashType &sigHashType) {

        // We only support this for now
        if(sigHashType.type() != SigHashType::MutuallyExclusiveType::all)
            throw std::runtime_error("unsupported sighash type, only sighash_all is supported");

        if(inputIndex >= tx.inputs.size()) {
            throw std::runtime_error("sighash: input index out of range");
        }

        // Make a copy of the transaction
        Coin::Transaction txCopy = tx;

        if(sigHashType.anyOneCanPay()) {
            // Sign only one input
            // https://en.bitcoin.it/wiki/OP_CHECKSIG#Procedure_for_Hashtype_SIGHASH_ANYONECANPAY

            // Remove all inputs
            txCopy.inputs.clear();

            // Add input to sign as input 0
            txCopy.inputs.push_back(tx.inputs[inputIndex]);

            // Set script sig to subscript for signature
            txCopy.inputs[0].scriptSig = subscript;

        } else {
            // Sign all inputs

            // Clear all inputs scripts
            txCopy.clearScriptSigs();

            // Set script sig to subscript for signature
            txCopy.inputs[inputIndex].scriptSig = subscript;
        }

        // Compute sighash
        return txCopy.getHashWithAppendedCode(sigHashType.hashCode());
    }

    uchar_vector sighash(const Coin::Transaction & tx,
                    const typesafeOutPoint &outPoint,
                    const uchar_vector & subscript,
                    const SigHashType & sigHashType) {

        // find input index by outpoint
        for(uint n = 0; n < tx.inputs.size(); n++) {
            if(typesafeOutPoint(tx.inputs[n].previousOut) == outPoint) {
                return sighash(tx, n, subscript, sigHashType);
            }
        }

        throw std::runtime_error("sighash: transaction does not have a corresponding input");
    }

    uchar_vector serializeForOP_CHECKSIGMULTISIG(const std::vector<TransactionSignature> & sigs) {

        uchar_vector serialized;

        // Test if there are any signatures at all before
        // adding leading OP_0
        if(sigs.size() > 0) {

            // Add leading OP_0 bug thing
            serialized.push_back(0x00); // OP_0

            // Add each signature and corresponding sighash flag
            for(std::vector<TransactionSignature>::const_iterator i = sigs.cbegin(),
                end = sigs.cend(); i != end; i++)
                serialized += (*i).opPushForScriptSigSerialized();

        } else
            assert(false);

        return serialized;

    }

    // Borrowed from bitcoin core Script.h CScript::serialize
    uchar_vector serializeScriptNum(const int64_t& value) {
       if(value == 0)
           return uchar_vector();

       uchar_vector result;
       const bool neg = value < 0;
       uint64_t absvalue = neg ? -value : value;

       while(absvalue)
       {
           result.push_back(absvalue & 0xff);
           absvalue >>= 8;
       }

//    - If the most significant byte is >= 0x80 and the value is positive, push a
//    new zero-byte to make the significant byte < 0x80 again.

//    - If the most significant byte is >= 0x80 and the value is negative, push a
//    new 0x80 byte that will be popped off when converting to an integral.

//    - If the most significant byte is < 0x80 and the value is negative, add
//    0x80 to it, since it will be subtracted and interpreted as a negative when
//    converting to an integral.

       if (result.back() & 0x80)
           result.push_back(neg ? 0x80 : 0);
       else if (neg)
           result.back() |= 0x80;

       return result;
    }

    // Borrowed from bitcoin core Script.h CScript::set_vch
    int64_t deserializeScriptNum(const uchar_vector & vch)
    {
        if (vch.empty())
         return 0;

        int64_t result = 0;
        for (size_t i = 0; i != vch.size(); ++i)
         result |= static_cast<int64_t>(vch[i]) << 8*i;

        // If the input vector's most significant byte is 0x80, remove it from
        // the result's msb and return a negative.
        if (vch.back() & 0x80)
         return -((int64_t)(result & ~(0x80ULL << (8 * (vch.size() - 1)))));

        return result;
    }
}
