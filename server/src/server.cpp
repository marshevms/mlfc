#include "server.h"

#include <QTimer>

#include "serveradapter.h"
#include "core/manager.h"

namespace mlfc
{

Server::Server(QObject *parent)
    : QObject(parent)
    , server_status_(ServerStatus::kOFF)
    , timer_(new QTimer(this))
    , manager_(nullptr)
{
}


Server::~Server()
{

}

bool Server::start()
{
    connect(timer_, &QTimer::timeout, this, &Server::update);

    try
    {
        manager_.reset(new mlfc::core::Manager());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        return false;
    }


    timer_->start(kTimeoutSecs);
    server_status_ = ServerStatus::kON;

    return true;
}

core::FanMode Server::fanMode()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return core::FanMode::Unknown;
    }

    try
    {
        return manager_->fanMode();
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return core::FanMode::Unknown;
}

core::CoolerBoost Server::coolerBoost()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return core::CoolerBoost::Unknown;
    }

    try
    {
        return manager_->coolerBoost();
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return core::CoolerBoost::Unknown;
}

bool Server::setCoolerBoost(core::CoolerBoost cooler_boost)
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return false;
    }

    try
    {
        manager_->setCoolerBoost(cooler_boost);

        return true;
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setFanMode(core::FanMode fan_mode)
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return false;
    }

    try
    {
        manager_->setFanMode(fan_mode);

        return true;
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

QVector<int> Server::cpuTemps()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return {};
    }

    try
    {
        auto cpuTemps = manager_->cpuTemps();
        return QVector<int>(cpuTemps.begin(), cpuTemps.end());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

QVector<int> Server::cpuFanSpeeds()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return {};
    }

    try
    {
        auto cpuFanSpeeds = manager_->cpuFanSpeeds();
        return QVector<int>(cpuFanSpeeds.begin(), cpuFanSpeeds.end());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

QVector<int> Server::gpuTemps()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return {};
    }

    try
    {
        auto gpuTemps = manager_->gpuTemps();
        return QVector<int>(gpuTemps.begin(), gpuTemps.end());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

QVector<int> Server::gpuFanSpeeds()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return {};
    }

    try
    {
        auto fanSpeeds = manager_->gpuFanSpeeds();
        return QVector<int>(fanSpeeds.begin(), fanSpeeds.end());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

bool Server::setCPUTemps(QVector<int> &cpuTemps)
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return false;
    }

    try
    {
        manager_->setCPUTemps({cpuTemps.begin(), cpuTemps.end()});
        return true;
    }
    catch (std::logic_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setCPUFanSpeeds(QVector<int> &cpuFanSpeeds)
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return false;
    }

    try
    {
        manager_->setCPUFanSpeeds({cpuFanSpeeds.begin(), cpuFanSpeeds.end()});
        return true;
    }
    catch (std::logic_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setGPUTemps(QVector<int> &gpuTemps)
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return false;
    }

    try
    {
        manager_->setGPUTemps({gpuTemps.begin(), gpuTemps.end()});
        return true;
    }
    catch (std::logic_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setGPUFanSpeeds(QVector<int> &gpuFanSpeeds)
{
    if(server_status_ == ServerStatus::kOFF)
    {
        setLastError({"Server is not started yet"});
        return false;
    }

    try
    {
        manager_->setGPUFanSpeeds({gpuFanSpeeds.begin(), gpuFanSpeeds.end()});
        return true;
    }
    catch (std::logic_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setUpdateInterval(int interval)
{
    if(interval < kMinTimeoutSecs && interval > kMaxTimeoutSecs)
        return false;

    timer_->setInterval(interval);

    return true;
}

int Server::updateInterval() const
{
    return timer_->interval();
}

QString Server::lastError() const
{
    return last_error_;
}

void Server::setLastError(const QString &error)
{
    last_error_ = error;
}

void Server::update()
{
    try
    {
        emit realtimeCPUTemp(manager_->realtimeCPUTemp());
        emit realtimeCPUFanRPM(manager_->realtimeCPUFanRPM());

        emit realtimeGPUTemp(manager_->realtimeGPUTemp());
        emit realtimeGPUFanRPM(manager_->realtimeGPUFanRPM());
    }
    catch (std::runtime_error &e)
    {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }
}


} // namespace mlfc
