#ifndef READER_H
#define READER_H

#include <fstream>
#include <vector>

#include "constants.h"

namespace mlfc::core
{

class Reader
{
public:
    Reader();
    ~Reader();

    //CPU
    int realtimeCPUTemp();
    int realtimeCPUFanRPM();
    std::vector<int> cpuTemps();
    std::vector<int> cpuFanSpeeds();

    //GPU
    int realtimeGPUTemp();
    int realtimeGPUFanRPM();
    std::vector<int> gpuTemps();
    std::vector<int> gpuFanSpeeds();

    enum FanMode fanMode();
    enum CoolerBoost coolerBoost();

    std::string ecVersion();

protected:
    enum FanModeType fanModeType();
    enum CpuIntelGen cpuIntelgen();

private:
    std::fstream file_;
    FanModeType fanModeType_;
    CpuIntelGen cpuIntelGen_;

    unsigned char fanModeValue();
    enum FanModeType defineFanModeType();
    enum CpuIntelGen defineCpuIntelGen();
};

} // namespace mlfc::core


#endif // READER_H
