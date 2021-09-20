#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>

namespace mlfc::core
{

#ifdef NDEBUG
static constexpr char kECFilePath[]{"/sys/kernel/debug/ec/ec0/io"};
#else
static constexpr char kECFilePath[]{"io"};
#endif

static constexpr int kTempsNumber = 6;
static constexpr int kFanSpeedsNumber = 7;

static constexpr int kFanRPM{478000};
static constexpr int kFanMode{0xF4};
static constexpr int kCoolerBoost{0x98};

struct cpu
{
    static constexpr int kRealtimeTemp{0x68};
    static constexpr std::array<int, 2> kRealtimeFanRPM{0xCC, 0xCD};
    static constexpr std::array<int, kTempsNumber> kTemps{0x6A, 0x6B, 0x6C
                                                          ,0x6D, 0x6E, 0x6F};
    static constexpr std::array<int, kFanSpeedsNumber> kFanSpeeds{0x72, 0x73, 0x74
                                                                  ,0x75, 0x76, 0x77, 0x78};
};

struct gpu
{
    static constexpr int kRealtimeTemp{0x80};
    static constexpr std::array<int, 2> kRealtimeFanRPM{0xCA, 0xCB};
    static constexpr std::array<int, kTempsNumber> kTemps{0x82, 0x83, 0x84
                                                          ,0x85, 0x86, 0x87};
    static constexpr std::array<int, kFanSpeedsNumber> kFanSpeeds{0x8A, 0x8B, 0x8C
                                                                  ,0x8F, 0x8E, 0x8F, 0x90};
};

constexpr int kCoolerBoostON{0x80};
constexpr int kCoolerBoostOFF{0x00};

enum class CoolerBoost
{
    ONN,
    OFF,
    Unknown
};

static constexpr std::array<int, 2>  kFanModeAuto{0x0C, 0x0D};
static constexpr std::array<int, 2>  kFanModeBasic{0x4C, 0x4D};
static constexpr std::array<int, 2>  kFanModeAdvanced{0x8C, 0x8D};

enum class FanModeType{
    TypeC = 0,
    TypeD,
};

enum class FanMode
{
    Auto,
    Basic,
    Advanced,
    Unknown
};

static constexpr std::pair<int, int> kECVersion{0xA0, 8};

} // namespace mlfc::core

#endif // CONSTANTS_H
