#ifndef COIN_RELATIVELOCKTIME_HPP
#define COIN_RELATIVELOCKTIME_HPP

#include <stdlib.h>
#include <chrono>

class uchar_vector;

namespace Coin {

class RelativeLockTime
{
public:

    typedef uint16_t LockTimeCounter;

    enum class Units {
        Blocks,
        Time
    };

    // Default Constructor
    RelativeLockTime();

    RelativeLockTime(const Units &, const LockTimeCounter &);

    static RelativeLockTime fromTimeUnits(const LockTimeCounter &);
    static RelativeLockTime fromSecondsDuration(const std::chrono::seconds &);
    static RelativeLockTime fromBlockUnits(const LockTimeCounter &);

    bool operator==(const RelativeLockTime & rhs) const;

    bool operator!=(const RelativeLockTime & rhs) const;

    bool operator<(const RelativeLockTime &rhs) const;

    bool operator>(const RelativeLockTime &rhs) const;

    LockTimeCounter counter() const;
    void counter(const LockTimeCounter &);

    void units(const Units &);
    Units units() const;

    // Returns time based locktime in seconds, throws if units is not time
    std::chrono::seconds getDuration() const;

    // Safe way to set units and counter
    void setTime(const LockTimeCounter &);
    void setBlocks(const LockTimeCounter &);
    void setDuration(const std::chrono::seconds &);

    //nSequence value to be used in a transaction input
    uint32_t toSequenceNumber() const;

    static RelativeLockTime fromSequenceNumber(const uint32_t &);

    //toScriptData const - return representation in bitcoin Script
    uchar_vector toScriptData() const;

    //static fromScriptData
    static RelativeLockTime fromScriptData(const uchar_vector &);

protected:
    static LockTimeCounter secondsToCounter(const std::chrono::seconds &);
    static std::chrono::seconds counterToSeconds(const LockTimeCounter &);

    LockTimeCounter _counter;
    Units _units;
};

}
#endif // COIN_RELATIVELOCKTIME_HPP
