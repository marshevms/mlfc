#include "reader.h"

#include <iostream>
#include <system_error>
#include <string>
#include <string.h>
#include <cpuid.h>

namespace mlfc::core
{

Reader::Reader()
{
    file_.exceptions(std::fstream::failbit);

    try
    {
        file_.open(kECFilePath, std::fstream::in | std::fstream::binary);
        fanModeType_ = defineFanModeType();
        cpuIntelGen_ = defineCpuIntelGen();
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("An attempt to open the \"" + std::string(kECFilePath) + "\" file resulted in an error: " + std::string(strerror(errno)));
    }
}

Reader::~Reader()
{

}

int Reader::realtimeCPUTemp()
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

int Reader::realtimeCPUFanRPM()
{
    try
    {
        file_.seekg(cpu::kRealtimeFanRPM[static_cast<int>(cpuIntelGen_)][0]);

        unsigned char data[cpu::kRealtimeFanRPM[static_cast<int>(cpuIntelGen_)].size()];
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

std::vector<int> Reader::cpuTemps()
{
    try
    {
        file_.seekg(cpu::kTemps[0]);

        unsigned char data[cpu::kTemps.size()];
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != cpu::kTemps.size())
            return {};

        std::vector<int> res;
        res.reserve(cpu::kTemps.size());

        for(size_t i = 0; i < cpu::kTemps.size(); ++i)
        {
            res.push_back(data[i]);
        }

        return res;
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading CPU temps: " + std::string(strerror(errno)));
    }
}

std::vector<int> Reader::cpuFanSpeeds()
{
    try
    {
        file_.seekg(cpu::kFanSpeeds[0]);

        unsigned char data[cpu::kFanSpeeds.size()];
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != cpu::kFanSpeeds.size())
            return {};

        std::vector<int> res;
        res.reserve(cpu::kFanSpeeds.size());

        for(size_t i = 0; i < cpu::kFanSpeeds.size(); ++i)
        {
            res.push_back(data[i]);
        }

        return res;
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading CPU fan speeds: " + std::string(strerror(errno)));
    }
}

int Reader::realtimeGPUTemp()
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

int Reader::realtimeGPUFanRPM()
{
    try
    {
        file_.seekg(gpu::kRealtimeFanRPM[static_cast<int>(cpuIntelGen_)][0]);

        unsigned char data[gpu::kRealtimeFanRPM[static_cast<int>(cpuIntelGen_)].size()];
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

std::vector<int> Reader::gpuTemps()
{
    try
    {
        file_.seekg(gpu::kTemps[0]);

        unsigned char data[gpu::kTemps.size()];
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != gpu::kTemps.size())
            return {};

        std::vector<int> res;
        res.reserve(gpu::kTemps.size());

        for(size_t i = 0; i < gpu::kTemps.size(); ++i)
        {
            res.push_back(data[i]);
        }

        return res;
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading GPU temps: " + std::string(strerror(errno)));
    }
}

std::vector<int> Reader::gpuFanSpeeds()
{
    try
    {
        file_.seekg(gpu::kFanSpeeds[0]);

        unsigned char data[gpu::kFanSpeeds.size()];
        file_.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file_.gcount() != gpu::kFanSpeeds.size())
            return {};

        std::vector<int> res;
        res.reserve(gpu::kFanSpeeds.size());

        for(size_t i = 0; i < gpu::kFanSpeeds.size(); ++i)
        {
            res.push_back(data[i]);
        }

        return res;
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading GPU fan speeds: " + std::string(strerror(errno)));
    }
}

FanMode Reader::fanMode()
{
    try
    {
        if (const auto value = static_cast<int>(fanModeValue()); value == kFanModeAuto[static_cast<int>(fanModeType_)]){
            return FanMode::Auto;
        } else if (value == kFanModeBasic[static_cast<int>(fanModeType_)]){
            return FanMode::Basic;
        } else if (value == kFanModeAdvanced[static_cast<int>(fanModeType_)]){
            return FanMode::Advanced;
        } else {
            throw std::runtime_error(std::string("While reading fan mode: ") + "unknown fan mode: " + std::to_string(value));
        }

    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading fan mode: " + std::string(strerror(errno)));
    }
}

CoolerBoost Reader::coolerBoost()
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

std::string Reader::ecVersion()
{
    try
    {
        char ec[kECVersion.second];

        file_.seekg(kECVersion.first);

        file_.read(ec, kECVersion.second);

        if(file_.gcount() != kECVersion.second)
            throw std::runtime_error(std::string("While reading ec version: ") + "read " + std::to_string(file_.gcount()) + " byte(s), "
                                     + "should read " + std::to_string(kECVersion.second) + "byte(s)");

        return {ec, kECVersion.second};

    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error("While reading ec version: " + std::string(strerror(errno)));
    }

    return {};
}

FanModeType Reader::fanModeType()
{
    return fanModeType_;
}

CpuIntelGen Reader::cpuIntelgen()
{
    return cpuIntelGen_;
}

unsigned char Reader::fanModeValue()
{
    unsigned char data = 0;

    file_.seekg(kFanMode);
    file_.read(reinterpret_cast<char*>(&data), sizeof (data));

    if(file_.gcount() != sizeof (data))
        throw std::runtime_error(std::string("While reading fan mode: ") + "read " + std::to_string(file_.gcount()) + " byte(s), "
                                 + "should read " + std::to_string(data) + "byte(s)");

    return data;
}

// TODO think about ec version
FanModeType Reader::defineFanModeType()
{
    static constexpr unsigned char mask = 0x3;   //      0000 0011
    auto data = fanModeValue();                  // OD - 0000 1101

    switch (auto value = data & mask; value) {
    case 0: return FanModeType::TypeC;
    case 1: return FanModeType::TypeD;
    }

    std::cerr << "fan_mode value is " << std::hex << static_cast<int>(data)  << ", please create the issue\n"
              << "on https://github.com/marshevms/mlfc/issues and mark it as \"unknown fan mode\"" << std:: endl;

    return FanModeType::TypeC;
}

CpuIntelGen Reader::defineCpuIntelGen()
{
    unsigned int eax = 0;
    unsigned int ebx = 0;
    unsigned int ecx = 0;
    unsigned int edx = 0;

    if (__get_cpuid(1, &eax, &ebx, &ecx, &edx) == 0){
        std::cerr << "failed to check cpuid";
        return CpuIntelGen::Default;
    }

    unsigned int efamily = (eax >> 20) & 0xFF;
    unsigned int emodel = (eax >> 16) & 0xF;
    unsigned int family = (eax >> 8) & 0xF;
    unsigned int model = (eax >> 4) & 0xF;

    // Rocket Lake
    if (efamily == 0 && family == 6 && emodel == 10 && model == 7){
        return CpuIntelGen::Gen11;
    }

    // Tiger Lake
    if (efamily == 0 && family == 6 && emodel == 8 && model == 12){
        return CpuIntelGen::Gen11;
    }
    if (efamily == 0 && family == 6 && emodel == 8 && model == 13){
        return CpuIntelGen::Gen11;
    }

    return CpuIntelGen::Default;
}

} // namespace mlfc::core
