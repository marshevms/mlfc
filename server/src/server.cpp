#include "server.h"

#include <QTimer>

#include "serveradapter.h"
#include "core/reader.h"


namespace mlfc
{

Server::Server(QObject *parent)
    : QObject(parent)
    , timer_(new QTimer(this))
    , reader_(nullptr)
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
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        return false;
    }


    timer_->start(kTimeoutSecs);

    return true;
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
