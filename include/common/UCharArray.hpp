/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_UCHAR_ARRAY_HPP
#define COIN_UCHAR_ARRAY_HPP

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <stdutils/uchar_vector.h>
#include <sstream> // stringstream

#include <common/Utilities.hpp>

namespace Coin {

// Forward declaration for stream operators
template<unsigned int array_length>
class UCharArray;

// Stream operator
template<unsigned int array_length>
std::ostream & operator<<(std::ostream & stream, const UCharArray<array_length> & o);

template<unsigned int array_length>
std::istream & operator>>(std::istream & stream, UCharArray<array_length> & o);

// (type safe length) fixed length array of unsigned chars
template<unsigned int array_length>
class UCharArray : public std::array<unsigned char, array_length> {

public:

    // Default constructor
    UCharArray();

    UCharArray(const std::vector<unsigned char>&);
    UCharArray(const std::string&);

    std::vector<unsigned char> getRawVector() const;

    // Encodes as hex string
    std::string getRawHex() const;

    // Return length
    static unsigned int rawLength();

    // Clear array, i.e. , set to 0s
    void clear();

    // Check if array is clear
    bool isClear() const;

    // Stream operator
    friend std::ostream & operator<< <array_length> (std::ostream & stream, const UCharArray<array_length> & o);
    friend std::istream & operator>> <array_length> (std::istream & stream, UCharArray<array_length> & o);

protected:
    void setRawHex(const std::string&);

    // Try to fill array with content starting at start
    void setRaw(const unsigned char* start);

    // Copy data from vector into array - size is checked
    void setRaw(const std::vector<unsigned char>&);

    // Reverses representation, to switch between little/big-endian
    void reverse();
};


template<unsigned int array_length>
UCharArray<array_length>::UCharArray() {
    clear();
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const std::vector<unsigned char>& rawBytes) {
    setRaw(rawBytes);
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const std::string &hexString) {
    setRawHex(hexString);
}

template<unsigned int array_length>
void UCharArray<array_length>::reverse() {
    std::reverse(this->begin(), this->end());
}

template<unsigned int array_length>
unsigned int UCharArray<array_length>::rawLength() {
    return array_length;
}

template<unsigned int array_length>
void UCharArray<array_length>::clear() {

    for(unsigned int i = 0; i < array_length; i++)
        this->at(i) = 0;
}

template<unsigned int array_length>
bool UCharArray<array_length>::isClear() const {

    for(int i = 0; i < array_length; i++) {
        if(this[i] != 0)
            return false;
    }

    return true;
}

template<unsigned int array_length>
std::string UCharArray<array_length>::getRawHex() const {

    return Coin::toHex(this->data(), array_length);
}

template<unsigned int array_length>
std::vector<unsigned char> UCharArray<array_length>::getRawVector() const {

    std::vector<unsigned char> v;

    for(unsigned int i = 0; i < array_length; i++)
        v.push_back(this->at(i));

    return v;
}

template<unsigned int array_length>
void UCharArray<array_length>::setRawHex(const std::string &hexString) {

    // Check that string has correct length - must be zero padded
    if(hexString.size() != 2*array_length) {

        // Create error message
        std::stringstream err;

        err << "String argument is of incorrect length, should be "
          << 2*array_length
          << ", but was "
          << hexString.length()
          << ".";

        throw std::runtime_error(err.str());

    }

    auto bytes = Coin::fromHex(hexString);

    setRaw(bytes);
}

template<unsigned int array_length>
void UCharArray<array_length>::setRaw(const unsigned char * start) {

    // Copy content into array
    for(unsigned int i = 0; i < array_length; i++)
        this->at(i) = start[i];
}

template<unsigned int array_length>
void UCharArray<array_length>::setRaw(const std::vector<unsigned char> & rawBytes) {

    // Check that vector has correct length
    if(rawBytes.size() != array_length) {

        // Create error message
        std::stringstream err;

        err << "vector argument is of incorrect size, should be "
          << array_length
          << ", but was "
          << rawBytes.size()
          << ".";

        throw std::runtime_error(err.str());

    }

    setRaw(rawBytes.data());
}

template<unsigned int array_length>
std::ostream & operator<<(std::ostream& stream, const Coin::UCharArray<array_length> & o) {

    int bytesWritten;

    // Get initial position of pointer the stream
    auto before = stream.tellp();

    // Write to stream from buffer
    stream.write((const char *)(o.data()), array_length);

    // Get position of pointer after writing to stream
    auto after = stream.tellp();

    if(after != -1 && before != -1)
        bytesWritten = after - before;
    else bytesWritten = -1;

    if(bytesWritten != array_length) {

        // Create error message
        std::stringstream s;

        s << "Failed writing" << array_length << " bytes to the stream.";

        throw new std::runtime_error(s.str());

    }

    return stream;
}

template<unsigned int array_length>
std::istream & operator>>(std::istream& stream, Coin::UCharArray<array_length> & o) {

    // Read from stream to array
    stream.read((char *)(o.data()), array_length);
    int bytesRead = stream.gcount();

    if(bytesRead != array_length) {

        // Create error message
        std::stringstream s;

        s << "Could not read " << array_length << " bytes from stream.";

        throw new std::runtime_error(s.str());
    }

    return stream;
}


}

#endif // COIN_UCHAR_ARRAY_HPP
