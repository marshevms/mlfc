#include "reader.h"

#include <iostream>
#include <system_error>
#include <string>
#include <string.h>

namespace mlfc
{

namespace core
{

Reader::Reader()
{
    file_.exceptions(std::fstream::failbit);

    try
    {
        file_.open(kECFilePath, std::fstream::in | std::fstream::binary);
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("An attempt to open the \"" + std::string(kECFilePath) + "\" file resulted in an error: " + std::string(strerror(errno)));
    }
}

Reader::~Reader()
{

}

int Reader::RealtimeCPUTemp()
{
    try
    {
        file_.seekg(cpu::kRealtimeTemp);

        unsigned char data = 0;
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != sizeof (data))
            return -2;

        return static_cast<int>(data);
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading realtime CPU temp: " + std::string(strerror(errno)));
    }
}

int Reader::RealtimeCPUFanRPM()
{
    try
    {
        file_.seekg(cpu::kRealtimeFanRPM[0]);

        unsigned char data[cpu::kRealtimeFanRPM.size()];
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != sizeof (data))
            return -2;

        int rpm = ((0 | data[0]) << 8) | data[1];

        if(!rpm)
            return 0;

        return kFanRPM / rpm;
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading realtime CPU fan rpm: " + std::string(strerror(errno)));
    }
}

int Reader::RealtimeGPUTemp()
{
    try
    {
        file_.seekg(gpu::kRealtimeTemp);

        unsigned char data = 0;
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != sizeof (data))
            return -2;

        return static_cast<int>(data);
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading realtime GPU temp: " + std::string(strerror(errno)));
    }
}

int Reader::RealtimeGPUFanRPM()
{
    try
    {
        file_.seekg(gpu::kRealtimeFanRPM[0]);

        unsigned char data[gpu::kRealtimeFanRPM.size()];
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != sizeof (data))
            return -2;

        int rpm = ((0 | data[0]) << 8) | data[1];

        if(!rpm)
            return 0;

        return kFanRPM / rpm;
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading realtime GPU fan rpm: " + std::string(strerror(errno)));
    }
}

FanMode Reader::FanMode()
{
    try
    {
        file_.seekg(kFanMode);

        unsigned char data = 0;
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != sizeof (data))
            throw std::runtime_error(std::string("While reading fan mode: ") + "read " + std::to_string(file_.gcount()) + " byte(s), "
                                     + "should read " + std::to_string(data) + "byte(s)");

        switch (static_cast<int>(data))
        {
        case kFanModeAuto:
            return FanMode::Auto;
        case kFanModeBasic:
            return FanMode::Basic;
        case kFanModeAdvanced:
            return FanMode::Advanced;
        default:
            throw std::runtime_error(std::string("While reading fan mode: ") + "unknown fan mode: " + std::to_string(static_cast<int>(data)));
        }

    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading fan mode: " + std::string(strerror(errno)));
    }
}

CoolerBoost Reader::CoolerBoost()
{
    try
    {
        file_.seekg(kCoolerBoost);

        unsigned char data = 0;
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != sizeof (data))
            throw std::runtime_error(std::string("While reading cooler boost: ") + "read " + std::to_string(file_.gcount()) + " byte(s), "
                                     + "should read " + std::to_string(data) + "byte(s)");

        if(!(static_cast<int>(data) & kCoolerBoostON))
            return CoolerBoost::OFF;
        else
            return CoolerBoost::ONN;

    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading cooler boost: " + std::string(strerror(errno)));
    }
}

} // namespace core

} // namespace mlfc
