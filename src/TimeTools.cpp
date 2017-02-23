/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#include <common/TimeTools.hpp>

#include <cmath>
#include <ctime>
#include <chrono>

namespace TimeTools {

uint8_t hoursInSeconds(uint32_t seconds) {
    return (uint8_t)floor(seconds / 3600);
}

uint8_t minutesInSeconds(uint32_t seconds) {
    return (uint8_t)floor(((seconds - 3600 * hoursInSeconds(seconds)) / 60));
}

uint8_t secondsInSeconds(uint32_t seconds) {
    return uint8_t(seconds - hoursInSeconds(seconds)*3600 - minutesInSeconds(seconds)*60);
}

/*
QTime secondsToQTime(uint32_t seconds) {

    uint8_t h = hoursInSeconds(seconds);
    uint8_t m = minutesInSeconds(seconds);
    uint8_t s = secondsInSeconds(seconds);

    return QTime(h,m,s);
}
*/

}
