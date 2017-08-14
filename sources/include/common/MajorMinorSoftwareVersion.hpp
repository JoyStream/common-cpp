/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 4 2016
 */

#ifndef JOYSTREAM_COMMON_MAJORMINORSOFTWAREVERSION_HPP
#define JOYSTREAM_COMMON_MAJORMINORSOFTWAREVERSION_HPP

#include <string>
#include <stdexcept>

namespace joystream {
namespace common {

    class MajorMinorSoftwareVersion {

    public:

        struct InvalidProtocolVersionStringException : std::runtime_error {

            InvalidProtocolVersionStringException(const std::string & s)
                : std::runtime_error(std::string("Invalid encoding") + s)
                , versionString(s) {
            }

            std::string versionString;
        };

        MajorMinorSoftwareVersion()
            : MajorMinorSoftwareVersion(0,0) {
        }

        MajorMinorSoftwareVersion(unsigned int major, unsigned int minor)
            : _major(major)
            , _minor(minor) {
        }

        static MajorMinorSoftwareVersion fromString(const std::string & s) {

            std::size_t found = s.find(".");

            if(found == std::string::npos)
                throw InvalidProtocolVersionStringException(s);

            std::string majorString = s.substr(0, found);
            std::string minorString = s.substr(found + 1, s.length() - (found + 1));

            return MajorMinorSoftwareVersion((unsigned int)std::stoi(majorString.c_str()),
                                   (unsigned int)std::stoi(minorString.c_str()));
        }

        std::string toString() const {
            return std::to_string(_major) + "." + std::to_string(_minor);
        }

    private:

        unsigned int _major;
        unsigned int _minor;
    };
}
}

#endif // JOYSTREAM_COMMON_MAJORMINORSOFTWAREVERSION_HPP
