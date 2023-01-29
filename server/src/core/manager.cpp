#include "manager.h"

#include <fstream>
#include <sstream>
#include <string.h>

namespace mlfc::core {

Manager::Manager()
    : Reader()
{
}

void Manager::setCoolerBoost(enum CoolerBoost coolerboost)
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    try {
        file.open(cooler_boost_status_path, file.out);

        switch (coolerboost) {
        case CoolerBoost::ONN:
            file << "onn";
            break;
        case CoolerBoost::OFF:
            file << "off";
            break;
        case CoolerBoost::Unknown:
            break;
        }

    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::setFanMode(enum FanMode fan_mode)
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    try {
        file.open(fan_mode_path, file.out);

        switch (fan_mode) {
        case FanMode::Auto:
            file << "auto";
            break;
        case FanMode::Basic:
            file << "basic";
            break;
        case FanMode::Advanced:
            file << "advanced";
            break;
        case FanMode::Unknown:
            break;
        }

    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::setCPUTemps(const std::vector<int>& cpuTemps)
{
    if (cpuTemps.size() == 0) {
        return;
    }

    std::string res;
    res.reserve(cpuTemps.size() * 3 + (cpuTemps.size() - 1));
    res.append(std::to_string(cpuTemps[0]));

    for (auto it = ++cpuTemps.begin(); it != cpuTemps.end(); ++it) {
        res.append(",");
        res.append(std::to_string(*it));
    }

    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    try {
        file.open(cpu_temps_path, file.out);
        file << res;

    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::setCPUFanSpeeds(const std::vector<int>& cpuFanSpeeds)
{
    if (cpuFanSpeeds.size() == 0) {
        return;
    }

    std::string res;
    res.reserve(cpuFanSpeeds.size() * 3 + (cpuFanSpeeds.size() - 1));
    res.append(std::to_string(cpuFanSpeeds[0]));

    for (auto it = ++cpuFanSpeeds.begin(); it != cpuFanSpeeds.end(); ++it) {
        res.append(",");
        res.append(std::to_string(*it));
    }

    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    try {
        file.open(cpu_fan_speeds_path, file.out);
        file << res;

    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::setGPUTemps(const std::vector<int>& gpuTemps)
{
    if (gpuTemps.size() == 0) {
        return;
    }

    std::string res;
    res.reserve(gpuTemps.size() * 3 + (gpuTemps.size() - 1));
    res.append(std::to_string(gpuTemps[0]));

    for (auto it = ++gpuTemps.begin(); it != gpuTemps.end(); ++it) {
        res.append(",");
        res.append(std::to_string(*it));
    }

    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    try {
        file.open(gpu_temps_path, file.out);
        file << res;

    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

void Manager::setGPUFanSpeeds(const std::vector<int>& gpuFanSpeeds)
{
    if (gpuFanSpeeds.size() == 0) {
        return;
    }

    std::string res;
    res.reserve(gpuFanSpeeds.size() * 3 + (gpuFanSpeeds.size() - 1));
    res.append(std::to_string(gpuFanSpeeds[0]));

    for (auto it = ++gpuFanSpeeds.begin(); it != gpuFanSpeeds.end(); ++it) {
        res.append(",");
        res.append(std::to_string(*it));
    }

    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    try {
        file.open(gpu_fan_speeds_path, file.out);
        file << res;

    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

} // namespace mlfc::core
