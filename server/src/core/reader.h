#ifndef READER_H
#define READER_H

#include <string>
#include <vector>

#include "constants.h"

namespace mlfc::core {

class Reader {
public:
    Reader();
    ~Reader();

    // CPU
    int realtimeCPUTemp();
    int realtimeCPUFanRPM();
    std::vector<int> cpuTemps();
    std::vector<int> cpuFanSpeeds();

    // GPU
    int realtimeGPUTemp();
    int realtimeGPUFanRPM();
    std::vector<int> gpuTemps();
    std::vector<int> gpuFanSpeeds();

    enum FanMode fanMode();
    enum CoolerBoost coolerBoost();

    std::string ecVersion();
};

} // namespace mlfc::core

#endif // READER_H
