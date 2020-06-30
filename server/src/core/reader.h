#ifndef READER_H
#define READER_H

#include <fstream>

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

    //GPU
    int RealtimeGPUTemp();
    int RealtimeGPUFanRPM();

    enum FanMode FanMode();
    enum CoolerBoost CoolerBoost();

private:
    std::fstream file_;
};

} // namespace core

} // namespace mlfc


#endif // READER_H
