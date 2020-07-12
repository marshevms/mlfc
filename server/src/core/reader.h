#ifndef READER_H
#define READER_H

#include <fstream>
#include <vector>

#include "constants.h"

namespace mlfc
{

namespace core
{

class Reader
{
public:
    Reader();
    ~Reader();

    //CPU
    int RealtimeCPUTemp();
    int RealtimeCPUFanRPM();
    std::vector<int> CPUTemps();
    std::vector<int> CPUFanSpeeds();

    //GPU
    int RealtimeGPUTemp();
    int RealtimeGPUFanRPM();
    std::vector<int> GPUTemps();
    std::vector<int> GPUFanSpeeds();

    enum FanMode FanMode();
    enum CoolerBoost CoolerBoost();

private:
    std::fstream file_;
};

} // namespace core

} // namespace mlfc


#endif // READER_H
