#ifndef MANAGER_H
#define MANAGER_H

#include "constants.h"

namespace mlfc
{

namespace core
{

class Manager
{
public:
    Manager() = default;
    ~Manager() = default;

    void SetCoolerBoost(CoolerBoost coolerboost);

};

} // namespace core

} // namespace mlfc

#endif // MANAGER_H
