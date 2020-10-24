#ifndef MANAGER_H
#define MANAGER_H

#include "constants.h"
#include "reader.h"

namespace mlfc::core
{

class Manager : public Reader
{
public:
    Manager();
    ~Manager() = default;

    void setCoolerBoost(enum CoolerBoost coolerboost);
    void setFanMode(enum FanMode fan_mode);

private:
    void openFileRW(std::fstream &file);
};

} // namespace mlfc::core

#endif // MANAGER_H
