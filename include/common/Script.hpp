/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#ifndef COIN_SCRIPT_HPP
#define COIN_SCRIPT_HPP

class uchar_vector;

namespace Coin {

class Script
{
public:

    virtual uchar_vector serialize() const = 0;

};

}

#endif // COIN_SCRIPT_HPP
