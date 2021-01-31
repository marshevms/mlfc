#include "gpu.h"

#include "constants.h"

namespace mlfc
{

GPU::GPU(QObject *parent)
    : QObject(parent)
    , temp_(0)
    , fanRpm_(0)
{
    temps_.resize(kTempsNumber);
    fanSpeeds_.resize(kFanSpeedsNumber);
}

int GPU::setTemp(int temp)
{
    temp_ = temp;
    emit tempChanged();

    return 0;
}

int GPU::temp() const
{
    return temp_;
}

int GPU::setFanRpm(int fanRpm)
{
    fanRpm_ = fanRpm;
    emit fanRpmChanged();

    return 0;
}

int GPU::fanRpm() const
{
    return fanRpm_;
}

int GPU::setTemps(const QVector<int> &temps)
{
    if (temps.size() != kTempsNumber)
    {
        return -1;
    }

    for(int i = 0; i < kTempsNumber; ++i)
    {
        temps_[i] = temps[i];
    }

    emit tempsChanged();

    return 0;
}

QVector<int> GPU::temps() const
{
    return temps_;
}

const QVector<int> &GPU::rTemps() const
{
    return temps_;
}

int GPU::setFanSpeeds(const QVector<int> &fanSpeeds)
{
    if (fanSpeeds.size() != kFanSpeedsNumber)
    {
        return -1;
    }

    for(int i = 0; i < kFanSpeedsNumber; ++i)
    {
        fanSpeeds_[i] = fanSpeeds[i];
    }

    emit fanSpeedsChanged();

    return 0;
}

QVector<int> GPU::fanSpeeds() const
{
    return fanSpeeds_;
}

const QVector<int> &GPU::rFanSpeeds() const
{
    return fanSpeeds_;
}

} // namespace mlfc
