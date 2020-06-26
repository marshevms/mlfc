#include "gpu.h"

namespace mlfc
{

GPU::GPU(QObject *parent)
    : QObject(parent)
    , temp_(0)
    , fan_rpm_(0)
{

}

int GPU::set_temp(int temp)
{
    temp_ = temp;
    emit temp_changed();
}

int GPU::temp() const
{
    return temp_;
}

int GPU::set_fan_rpm(int fan_rpm)
{
    fan_rpm_ = fan_rpm;
    emit fan_rpm_changed();
}

int GPU::fan_rpm() const
{
    return fan_rpm_;
}

} // namespace mlfc
