#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>

namespace mlfc
{

namespace core
{

#ifdef NDEBUG
static constexpr char kECFilePath[]{"/sys/kernel/debug/ec/ec0/io"};
#else
static constexpr char kECFilePath[]{"io"};
#endif

static constexpr int kFanRPM = 478000;

struct cpu
{
    static constexpr int kRealtimeTemp{0x68};
    static constexpr std::array<int, 2> kRealtimeFanRPM{0xCC, 0xCD};
};

struct gpu
{
    static constexpr int kRealtimeTemp{0x80};
    static constexpr std::array<int, 2> kRealtimeFanRPM{0xCA, 0xCB};
};

} // namespace core

} // namespace mlfc

#endif // CONSTANTS_H
