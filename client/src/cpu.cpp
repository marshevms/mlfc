#include "cpu.h"

#include "constants.h"

namespace mlfc
{

CPU::CPU(QObject *parent)
    : QObject(parent)
    , temp_(0)
    , fanRpm_(0)
{
    temps_.resize(kTempsNumber);
    fanSpeeds_.resize(kFanSpeedsNumber);
}

int CPU::setTemp(int temp)
{
    temp_ = temp;
    emit tempChanged();

    return 0;
}

int CPU::temp() const
{
    return temp_;
}

int CPU::setFanRpm(int fanRpm)
{
    fanRpm_ = fanRpm;
    emit fanRpmChanged();

    return 0;
}

int CPU::fanRpm() const
{
    return fanRpm_;
}

int CPU::setTemps(const QVector<int> &temps)
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

QVector<int> CPU::temps()
{
    return temps_;
}

int CPU::setFanSpeeds(const QVector<int> &fanSpeeds)
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

QVector<int> CPU::fanSpeeds()
{
    return fanSpeeds_;
}

} // namespace mlfc
