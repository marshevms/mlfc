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

bool Server::Start()
{
    connect(timer_, &QTimer::timeout, this, &Server::Update);

    try
    {
        manager_.reset(new mlfc::core::Manager());
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        return false;
    }


    timer_->start(kTimeoutSecs);
    server_status_ = ServerStatus::kON;

    return true;
}

core::FanMode Server::FanMode()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        set_last_error({"Server is not started yet"});
        return core::FanMode::Unknown;
    }

    try
    {
        return manager_->FanMode();
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }

    return core::FanMode::Unknown;
}

core::CoolerBoost Server::CoolerBoost()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        set_last_error({"Server is not started yet"});
        return core::CoolerBoost::Unknown;
    }

    try
    {
        return manager_->CoolerBoost();
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }

    return core::CoolerBoost::Unknown;
}

bool Server::SetCoolerBoost(core::CoolerBoost cooler_boost)
{
    if(server_status_ == ServerStatus::kOFF)
    {
        set_last_error({"Server is not started yet"});
        return false;
    }

    try
    {
        manager_->SetCoolerBoost(cooler_boost);

        return true;
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }

    return false;
}

bool Server::SetFanMode(core::FanMode fan_mode)
{
    if(server_status_ == ServerStatus::kOFF)
    {
        set_last_error({"Server is not started yet"});
        return false;
    }

    try
    {
        manager_->SetFanMode(fan_mode);

        return true;
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }

    return false;
}

QVector<int> Server::CPUTemps()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        set_last_error({"Server is not started yet"});
        return {};
    }

    try
    {
        return QVector<int>::fromStdVector(manager_->CPUTemps());
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }

    return {};
}

QVector<int> Server::CPUFanSpeeds()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        set_last_error({"Server is not started yet"});
        return {};
    }

    try
    {
        return QVector<int>::fromStdVector(manager_->CPUFanSpeeds());
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }

    return {};
}

QVector<int> Server::GPUTemps()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        set_last_error({"Server is not started yet"});
        return {};
    }

    try
    {
        return QVector<int>::fromStdVector(manager_->GPUTemps());
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }

    return {};
}

QVector<int> Server::GPUFanSpeeds()
{
    if(server_status_ == ServerStatus::kOFF)
    {
        set_last_error({"Server is not started yet"});
        return {};
    }

    try
    {
        return QVector<int>::fromStdVector(manager_->GPUFanSpeeds());
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }

    return {};
}

bool Server::SetUpdateInterval(int interval)
{
    if(interval < kMinTimeoutSecs && interval > kMaxTimeoutSecs)
        return false;

    timer_->setInterval(interval);

    return true;
}

int Server::UpdateInterval() const
{
    return timer_->interval();
}

QString Server::last_error() const
{
    return last_error_;
}

void Server::set_last_error(const QString &error)
{
    last_error_ = error;
}

void Server::Update()
{
    try
    {
        emit RealtimeCPUTemp(manager_->RealtimeCPUTemp());
        emit RealtimeCPUFanRPM(manager_->RealtimeCPUFanRPM());

        emit RealtimeGPUTemp(manager_->RealtimeGPUTemp());
        emit RealtimeGPUFanRPM(manager_->RealtimeGPUFanRPM());
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }
}


} // namespace mlfc
