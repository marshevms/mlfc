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
    connect(timer_, &QTimer::timeout, this, &Server::update);

    try
    {
        reader_.reset(new mlfc::core::Reader());
    }
    catch (std::runtime_error &e)
    {
        set_last_error(e.what());
        return false;
    }


    timer_->start(timeout_msecs);

    return true;
}

//bool Server::setTimeEnterval(int msecs)
//{
//    qDebug() << "set intervalsaada" << msecs;
//    if(!timer_)
//        return false;

//    timer_->setInterval(msecs);
//    qDebug() << "set interval" << msecs;

//    return true;
//}

QString Server::last_error() const
{
    return last_error_;
}

void Server::set_last_error(const QString &error)
{
    last_error_ = error;
}

void Server::update()
{
    emit RealtimeCPUTemp(reader_->RealtimeCPUTemp());
    emit RealtimeCPUFanRPM(reader_->RealtimeCPUFanRPM());

    emit RealtimeGPUTemp(reader_->RealtimeGPUTemp());
    emit RealtimeGPUFanRPM(reader_->RealtimeGPUFanRPM());
}


} // namespace mlfc
