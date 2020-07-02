#ifndef MANAGER_H
#define MANAGER_H

#include "constants.h"
#include "reader.h"

namespace mlfc
{

namespace core
{

class Manager : public Reader
{
public:
    Manager();
    ~Manager() = default;

    void SetCoolerBoost(enum CoolerBoost coolerboost);
    void SetFanMode(enum FanMode fan_mode);

private:
    void openFileRW(std::fstream &file);
};

} // namespace core

} // namespace mlfc

#endif // MANAGER_H
