#include "manager.h"

#include <fstream>
#include <string.h>

namespace mlfc
{

namespace core
{

void Manager::SetCoolerBoost(CoolerBoost coolerboost)
{
    std::fstream file;
    file.exceptions(std::fstream::failbit);

    try
    {
        file.open(kECFilePath, std::fstream::in | std::fstream::out | std::fstream::binary);

        file.seekg(kCoolerBoost);

        unsigned char data = 0;
        file.read(reinterpret_cast<char*>(&data), sizeof (data));

        if(file.gcount() != sizeof (data))
            throw std::runtime_error(std::string("While reading cooler boost: ") + "read " + std::to_string(file.gcount()) + " byte(s), "
                                     + "should read " + std::to_string(data) + "byte(s)");

        CoolerBoost target = CoolerBoost::Unknown;
        if(!(static_cast<int>(data) & kCoolerBoostON))
            target = CoolerBoost::OFF;
        else
            target = CoolerBoost::ONN;

        if(target != coolerboost)
        {
            if(coolerboost == CoolerBoost::ONN)
            {
                data |= static_cast<unsigned char>(kCoolerBoostON);
            }
            else if(coolerboost == CoolerBoost::OFF)
            {
                data &= static_cast<unsigned char>(kCoolerBoostOFF) | 0x0F;
            }

            file.seekp(kCoolerBoost);
            file.write(reinterpret_cast<char*>(&data), sizeof (data));
            file.close();
        }
    }
    catch (std::ios_base::failure &e)
    {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

} // namespace core

} // namespace mlfc
