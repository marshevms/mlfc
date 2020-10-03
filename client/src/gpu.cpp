#include "gpu.h"

namespace mlfc
{

GPU::GPU(QObject *parent)
    : QObject(parent)
    , temp_(0)
    , fanRpm_(0)
{

}

int GPU::setTemp(int temp)
{
    temp_ = temp;
    emit tempChanged();
}

int GPU::temp() const
{
    return temp_;
}

int GPU::setFanRpm(int fanRpm)
{
    fanRpm_ = fanRpm;
    emit fanRpmChanged();
}

int GPU::fanRpm() const
{
    return fanRpm_;
}

} // namespace mlfc
