#include "manager.h"

#include <fstream>
#include <string.h>

namespace mlfc
{

namespace core
{

Manager::Manager() : Reader()
{

}

void Manager::SetCoolerBoost(enum CoolerBoost coolerboost)
{
    if(this->CoolerBoost() == coolerboost)
        return;

    try
    {
        std::fstream file;
        openFileRW(file);

        unsigned char data = 0;

        if(coolerboost == CoolerBoost::ONN)
        {
            data |= static_cast<unsigned char>(kCoolerBoostON);
        }
        else if(coolerboost == CoolerBoost::OFF)
        {
            data &= static_cast<unsigned char>(kCoolerBoostOFF) | 0x0F;
        }
        else
        {
            return;
        }

        file.seekp(kCoolerBoost);
        file.write(reinterpret_cast<char*>(&data), sizeof (data));
        file.close();
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::SetFanMode(enum FanMode fan_mode)
{
    if(this->FanMode() == fan_mode)
        return;

    try
    {
        std::fstream file;
        openFileRW(file);

        unsigned char data = 0;

        switch (fan_mode)
        {
        case FanMode::Auto:
            data = kFanModeAuto;
            break;
        case FanMode::Basic:
            data = kFanModeBasic;
            break;
        case FanMode::Advanced:
            data = kFanModeAdvanced;
            break;
        case FanMode::Unknown:
            return;
        }

        file.seekp(kFanMode);
        file.write(reinterpret_cast<char*>(&data), sizeof (data));
        file.close();
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::openFileRW(std::fstream &file)
{
    file.exceptions(std::fstream::failbit);

    try
    {
        file.open(kECFilePath, std::fstream::in | std::fstream::out | std::fstream::binary);
    }
    catch(std::ios_base::failure &e)
    {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

} // namespace core

} // namespace mlfc
