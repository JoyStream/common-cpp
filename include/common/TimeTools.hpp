/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#ifndef COIN_TIME_TOOLS_HPP
#define COIN_TIME_TOOLS_HPP

#include <cstdint>

namespace TimeTools {

// Converts x-> #units in x
uint8_t hoursInSeconds(uint32_t seconds);
uint8_t minutesInSeconds(uint32_t seconds);
uint8_t secondsInSeconds(uint32_t seconds);

//QTime secondsToQTime(uint32_t seconds);

}

#endif // COIN_TIME_TOOLS_HPP

