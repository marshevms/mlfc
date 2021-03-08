#include "serveradapter.h"

#include "server.h"

namespace mlfc
{

ServerAdapter::ServerAdapter(QObject *parent)
    :QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

bool ServerAdapter::start()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->start();
}

core::FanMode ServerAdapter::fanMode()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->fanMode();
}

core::CoolerBoost ServerAdapter::coolerBoost()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->coolerBoost();
}

bool ServerAdapter::setCoolerBoost(core::CoolerBoost cooler_boost)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->setCoolerBoost(cooler_boost);
}

bool ServerAdapter::setFanMode(core::FanMode fan_mode)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->setFanMode(fan_mode);
}

QVector<int> ServerAdapter::cpuTemps()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->cpuTemps();
}

QVector<int> ServerAdapter::cpuFanSpeeds()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->cpuFanSpeeds();
}

QVector<int> ServerAdapter::gpuTemps()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->gpuTemps();
}

QVector<int> ServerAdapter::gpuFanSpeeds()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->gpuFanSpeeds();
}

bool ServerAdapter::setCPUTemps(QVector<int> cpuTemps)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->setCPUTemps(cpuTemps);
}

bool ServerAdapter::setCPUFanSpeeds(QVector<int> cpuFanSpeeds)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->setCPUFanSpeeds(cpuFanSpeeds);
}

bool ServerAdapter::setGPUTemps(QVector<int> gpuTemps)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->setGPUTemps(gpuTemps);
}

bool ServerAdapter::setGPUFanSpeeds(QVector<int> gpuFanSpeeds)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->setGPUFanSpeeds(gpuFanSpeeds);
}

QString ServerAdapter::ecVersion()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->ecVersion();
}


bool ServerAdapter::setUpdateInterval(int interval)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->setUpdateInterval(interval);
}

int ServerAdapter::updateInterval() const
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->updateInterval();
}

QString ServerAdapter::lastError() const
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->lastError();
}

} // namespace mlfc
