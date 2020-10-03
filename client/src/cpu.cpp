#include "cpu.h"

namespace mlfc
{

CPU::CPU(QObject *parent)
    : QObject(parent)
    , temp_(0)
    , fanRpm_(0)
{

}

int CPU::setTemp(int temp)
{
    temp_ = temp;
    emit tempChanged();
}

int CPU::temp() const
{
    return temp_;
}

int CPU::setFanRpm(int fanRpm)
{
    fanRpm_ = fanRpm;
    emit fanRpmChanged();
}

int CPU::fanRpm() const
{
    return fanRpm_;
}

} // namespace mlfc
