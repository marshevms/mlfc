#include "cpu.h"

namespace mlfc
{

CPU::CPU(QObject *parent)
    : QObject(parent)
    , temp_(0)
    , fan_rpm_(0)
{

}

int CPU::set_temp(int temp)
{
    temp_ = temp;
    emit temp_changed();
}

int CPU::temp() const
{
    return temp_;
}

int CPU::set_fan_rpm(int fan_rpm)
{
    fan_rpm_ = fan_rpm;
    emit fan_rpm_changed();
}

int CPU::fan_rpm() const
{
    return fan_rpm_;
}

} // namespace mlfc
