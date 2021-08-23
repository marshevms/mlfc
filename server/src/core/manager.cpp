#include "manager.h"

#include <fstream>
#include <string.h>
#include <sstream>

namespace mlfc::core
{

Manager::Manager() : Reader()
{
}

void Manager::setCoolerBoost(enum CoolerBoost coolerboost)
{
    if(this->coolerBoost() == coolerboost)
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

void Manager::setFanMode(enum FanMode fan_mode)
{
    if(this->fanMode() == fan_mode)
        return;

    try
    {
        std::fstream file;
        openFileRW(file);

        unsigned char data = 0;

        switch (fan_mode)
        {
        case FanMode::Auto:
            //TODO cast
            data = kFanModeAuto[static_cast<int>(fanModeType())];
            break;
        case FanMode::Basic:
            data = kFanModeBasic[static_cast<int>(fanModeType())];
            break;
        case FanMode::Advanced:
            data = kFanModeAdvanced[static_cast<int>(fanModeType())];
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

void Manager::setCPUTemps(const std::vector<int> &cpuTemps)
{
    if (cpuTemps.size() != kTempsNumber)
    {
        std::ostringstream out;
        out << "Size of cpu temps must be equeal " << kTempsNumber;
        for(auto const & temp : cpuTemps)
        {
            out << temp << "  ";
        }

        throw std::logic_error(out.str());
    }

    if (!checkArrayValue(cpuTemps))
    {
        throw  std::logic_error("Wrong set of cpu temps");
    }

    try
    {
        std::fstream file;
        openFileRW(file);

        file.seekp(cpu::kTemps[0]);

        for(auto const & temp : cpuTemps)
        {
            unsigned char data = temp;
            file.write(reinterpret_cast<char*>(&data), sizeof (data));
        }

        file.close();

    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::setCPUFanSpeeds(const std::vector<int> &cpuFanSpeeds)
{
    if (cpuFanSpeeds.size() != kFanSpeedsNumber)
    {
        std::ostringstream out;
        out << "Size of cpu fan speeds must be equeal " << kFanSpeedsNumber;

        throw std::logic_error(out.str());
    }

    if (!checkArrayValue(cpuFanSpeeds))
    {
        throw  std::logic_error("Wrong set of cpu fan speeds");
    }

    try
    {
        std::fstream file;
        openFileRW(file);

        file.seekp(cpu::kFanSpeeds[0]);

        for(auto const & fanSpeed : cpuFanSpeeds)
        {
            unsigned char data = fanSpeed;
            file.write(reinterpret_cast<char*>(&data), sizeof (data));
        }

        file.close();

    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::setGPUTemps(const std::vector<int> &gpuTemps)
{
    if (gpuTemps.size() != kTempsNumber)
    {
        std::ostringstream out;
        out << "Size of gpu temps must be equeal " << kTempsNumber;
        for(auto const & temp : gpuTemps)
        {
            out << temp << "  ";
        }

        throw std::logic_error(out.str());
    }

    if (!checkArrayValue(gpuTemps))
    {
        throw  std::logic_error("Wrong set of gpu temps");
    }

    try
    {
        std::fstream file;
        openFileRW(file);

        file.seekp(gpu::kTemps[0]);

        for(auto const & temp : gpuTemps)
        {
            unsigned char data = temp;
            file.write(reinterpret_cast<char*>(&data), sizeof (data));
        }

        file.close();

    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::setGPUFanSpeeds(const std::vector<int> &gpuFanSpeeds)
{
    if (gpuFanSpeeds.size() != kFanSpeedsNumber)
    {
        std::ostringstream out;
        out << "Size of gpu fan speeds must be equeal " << kFanSpeedsNumber;

        throw std::logic_error(out.str());
    }

    if (!checkArrayValue(gpuFanSpeeds))
    {
        throw  std::logic_error("Wrong set of gpu fan speeds");
    }

    try
    {
        std::fstream file;
        openFileRW(file);

        file.seekp(gpu::kFanSpeeds[0]);

        for(auto const & fanSpeed : gpuFanSpeeds)
        {
            unsigned char data = fanSpeed;
            file.write(reinterpret_cast<char*>(&data), sizeof (data));
        }

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

bool Manager::checkArrayValue(const std::vector<int> &values)
{
    int oldValue = 0;
    for(auto const &value : values)
    {
        if (value < oldValue || value > 100)
            return false;

        oldValue = value;
    }

    return true;
}

} // namespace mlfc::core
