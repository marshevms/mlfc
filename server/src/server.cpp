#include "server.h"

#include <QTimer>

#include "serveradapter.h"
#include "core/reader.h"
#include "core/manager.h"

namespace mlfc
{

Server::Server(QObject *parent)
    : QObject(parent)
    , server_status_(ServerStatus::kOFF)
    , timer_(new QTimer(this))
    , reader_(nullptr)
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
        reader_.reset(new mlfc::core::Reader());
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
        return reader_->FanMode();
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
        return reader_->CoolerBoost();
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
        emit RealtimeCPUTemp(reader_->RealtimeCPUTemp());
        emit RealtimeCPUFanRPM(reader_->RealtimeCPUFanRPM());

        emit RealtimeGPUTemp(reader_->RealtimeGPUTemp());
        emit RealtimeGPUFanRPM(reader_->RealtimeGPUFanRPM());
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        emit AnErrorOccured(last_error());
    }
}


} // namespace mlfc
