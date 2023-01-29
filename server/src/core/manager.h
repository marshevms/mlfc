#ifndef MANAGER_H
#define MANAGER_H

#include "constants.h"
#include "reader.h"

namespace mlfc::core {

class Manager : public Reader {
public:
    Manager();
    ~Manager() = default;

    void setCoolerBoost(enum CoolerBoost coolerboost);
    void setFanMode(enum FanMode fan_mode);

    // CPU
    void setCPUTemps(const std::vector<int>& cpuTemps);
    void setCPUFanSpeeds(const std::vector<int>& cpuFanSpeeds);

    // GPU
    void setGPUTemps(const std::vector<int>& gpuTemps);
    void setGPUFanSpeeds(const std::vector<int>& gpuFanSpeeds);

private:
    void openFileRW(std::fstream& file);

    bool checkArrayValue(const std::vector<int>& values);
};

} // namespace mlfc::core

#endif // MANAGER_H
