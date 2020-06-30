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

static constexpr int kFanRPM{478000};
static constexpr int kFanMode{0xF4};
static constexpr int kCoolerBoost{0x98};

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

constexpr int kCoolerBoostON{0x80};
constexpr int kCoolerBoostOFF{0x00};

enum class CoolerBoost
{
    ONN,
    OFF,
    Unknown
};

constexpr int kFanModeAuto{0x0C};
constexpr int kFanModeBasic{0x4C};
constexpr int kFanModeAdvanced{0x8C};

enum class FanMode
{
    Auto,
    Basic,
    Advanced,
    Unknown
};


} // namespace core

} // namespace mlfc

#endif // CONSTANTS_H
