#include "reader.h"

#include <iostream>
#include <system_error>
#include <string.h>

#include "constants.h"

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
        std::cout << strerror(errno) << std::endl;
        return -1;
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
        std::cout << strerror(errno) << std::endl;
        return -1;
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
        std::cout << strerror(errno) << std::endl;
        return -1;
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
        std::cout << strerror(errno) << std::endl;
        return -1;
    }
}

} // namespace core

} // namespace mlfc
