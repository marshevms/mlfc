#include "reader.h"

#include <cpuid.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <system_error>

namespace mlfc::core {

Reader::Reader() { }

Reader::~Reader() { }

int Reader::realtimeCPUTemp()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(3);

    try {
        file.open(realtime_cpu_temp_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    return std::stoi(buf);
}

int Reader::realtimeCPUFanRPM()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(5);

    try {
        file.open(realtime_cpu_fan_rpm_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    return std::stoi(buf);
}

std::vector<int> Reader::cpuTemps()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(6 * 3 + 6);

    try {
        file.open(cpu_temps_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    std::vector<int> res;
    res.reserve(6);

    size_t pos_start = 0;
    size_t pos_end = 0;
    std::string delim(",");
    while ((pos_end = buf.find(delim, pos_start)) != std::string::npos) {
        auto token = buf.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim.length();
        res.push_back(std::stoi(token));
    }
    if (pos_start < buf.size()){
        res.push_back(std::stoi(buf.substr(pos_start)));
    }

    return res;
}

std::vector<int> Reader::cpuFanSpeeds()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(7 * 3 + 7);

    try {
        file.open(cpu_fan_speeds_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    std::vector<int> res;
    res.reserve(7);

    size_t pos_start = 0;
    size_t pos_end = 0;
    std::string delim(",");
    while ((pos_end = buf.find(delim, pos_start)) != std::string::npos) {
        auto token = buf.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim.length();
        res.push_back(std::stoi(token));
    }
    if (pos_start < buf.size()){
        res.push_back(std::stoi(buf.substr(pos_start)));
    }

    return res;
    ;
}

int Reader::realtimeGPUTemp()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(3);

    try {
        file.open(realtime_gpu_temp_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    return std::stoi(buf);
}

int Reader::realtimeGPUFanRPM()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(5);
    try {
        file.open(realtime_gpu_fan_rpm_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    return std::stoi(buf);
}

std::vector<int> Reader::gpuTemps()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(6 * 3 + 6);

    try {

        file.open(gpu_temps_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    std::vector<int> res;
    res.reserve(6);

    size_t pos_start = 0;
    size_t pos_end = 0;
    std::string delim(",");
    while ((pos_end = buf.find(delim, pos_start)) != std::string::npos) {
        auto token = buf.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim.length();
        res.push_back(std::stoi(token));
    }
    if (pos_start < buf.size()){
        res.push_back(std::stoi(buf.substr(pos_start)));
    }

    return res;
}

std::vector<int> Reader::gpuFanSpeeds()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(7 * 3 + 7);

    try {
        file.open(gpu_fan_speeds_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    std::vector<int> res;
    res.reserve(7);

    size_t pos_start = 0;
    size_t pos_end = 0;
    std::string delim(",");
    while ((pos_end = buf.find(delim, pos_start)) != std::string::npos) {
        auto token = buf.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim.length();
        res.push_back(std::stoi(token));
    }
    if (pos_start < buf.size()){
        res.push_back(std::stoi(buf.substr(pos_start)));
    }

    return res;
}

FanMode Reader::fanMode()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(10);

    try {
        file.open(fan_mode_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    if ((buf.find("auto")) != std::string::npos) {
        return FanMode::Auto;
    }
    if ((buf.find("basic")) != std::string::npos) {
        return FanMode::Basic;
    }
    if ((buf.find("advanced")) != std::string::npos) {
        return FanMode::Advanced;
    }

    return FanMode::Unknown;
}

CoolerBoost Reader::coolerBoost()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(4);


    try {
        file.open(cooler_boost_status_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    if ((buf.find("onn")) != std::string::npos) {
        return CoolerBoost::ONN;
    }
    if ((buf.find("off")) != std::string::npos) {
        return CoolerBoost::OFF;
    }

    return CoolerBoost::Unknown;
}

std::string Reader::ecVersion()
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.rdbuf()->pubsetbuf(0, 0);

    std::string buf;
    buf.resize(20);

    try {
        file.open(ec_version_path, file.in);
        file.getline(buf.data(), buf.size());
    } catch (std::ios_base::failure& e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    return buf;
}

} // namespace mlfc::core
