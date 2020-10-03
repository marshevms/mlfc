#include "client.h"

#include <QTimer>
#include <QDebug>
#include <QMetaEnum>

#include "cpu.h"
#include "gpu.h"

namespace mlfc
{

Client::Client(QObject *parent)
    : QObject(parent)
    , serverState_(ServerStates::Unknown)
    , coolerBoost_(CoolerBoost::Unknown)
    , fanMode_(FanMode::Unknown)
{
}

bool Client::start(CPU *cpu, GPU *gpu)
{
    if(!QDBusConnection::systemBus().registerService("com.github.mlfc.client")
       || !QDBusConnection::systemBus().registerObject("/Client", this))
    {
        setLastError(QDBusConnection::systemBus().lastError().message());

        return false;
    }

    if(cpu == nullptr || gpu == nullptr)
        return false;

    cpu_ = cpu;
    gpu_ = gpu;

    server_ = new ServerInterface("com.github.mlfc.server", "/Server", QDBusConnection::systemBus(), this);

    connect(server_, &ServerInterface::RealtimeCPUTemp, this, &Client::setCpuTemp);
    connect(server_, &ServerInterface::RealtimeCPUFanRPM, this, &Client::setCpuFanRmp);

    connect(server_, &ServerInterface::RealtimeGPUTemp, this, &Client::setGpuTemp);
    connect(server_, &ServerInterface::RealtimeGPUFanRPM, this, &Client::setGpuFanRmp);

    tryStartServer();
    return true;
}

QString Client::lastError() const
{
    return lastError_;
}

void Client::setLastError(const QString &error)
{
    lastError_ = error;
}

Client::ServerStates Client::serverState()
{
    return serverState_;
}

Client::CoolerBoost Client::coolerBoost()
{
    return coolerBoost_;
}

Client::FanMode Client::fanMode()
{
    return fanMode_;
}

void Client::setCpuTemp(int temp)
{
    cpu_->setTemp(temp);
//    qDebug() << "cpu: " << temp;
}

void Client::setCpuFanRmp(int rpm)
{
    cpu_->setFanRpm(rpm);
//    qDebug() << "cpu: " << rpm;
}

void Client::setGpuTemp(int temp)
{
    gpu_->setTemp(temp);
//    qDebug() << "gpu: " << temp;
}

void Client::setGpuFanRmp(int rpm)
{
    gpu_->setFanRpm(rpm);
    //    qDebug() << "gpu: " << rpm;
}

void Client::setCoollerBoost(const mlfc::EnumerationStorage::CoolerBoost coolerBoost)
{
    auto res = server_->setCoolerBoost(coolerBoost);
    res.waitForFinished();

    if(res.isError())
    {
        emit errorOccurred(res.error().message());
        return;
    }

    if(!res.value())
    {
        emit errorOccurred(server_->lastError().value());
        return;
    }
}

void Client::setFanMode(const mlfc::EnumerationStorage::FanMode fanMode)
{
    if(fanMode_ != fanMode)
    {
        fanMode_ = fanMode;

        fanModeChanged();
    }
}

void Client::setFanMode(const int fanMode)
{
    auto res = server_->setFanMode(static_cast<FanMode>(fanMode));
    res.waitForFinished();

    if(res.isError())
    {
        emit errorOccurred(res.error().message());
        return;
    }

    if(!res.value())
    {
        emit errorOccurred(server_->lastError().value());
        return;
    }

    setFanMode(static_cast<FanMode>(fanMode));
}

void Client::setServerState(const mlfc::EnumerationStorage::ServerStates state)
{
    serverState_ = state;
    emit serverStateChanged();
}

void Client::tryStartServer()
{
    auto res = server_->start();

    setServerState(ServerStates::Starting);

    QTimer::singleShot(1000, this, [=](){

        if(res.isError())
        {
            setServerState(ServerStates::Stopped);
            emit errorOccurred(res.error().message());
            return;
        }

        if(res.value() == false)
        {
            setServerState(ServerStates::Stopped);
            emit errorOccurred(server_->lastError().value());
        }
        else
        {
            setServerState(ServerStates::Working);

            QTimer *timer = new QTimer(this);

            connect(timer, &QTimer::timeout, this, [=]{

                auto fanmode = server_->fanMode();
                auto coolerboost = server_->coolerBoost();

                fanmode.waitForFinished();
                coolerboost.waitForFinished();

                if(fanmode.isError())
                {
                    emit errorOccurred(res.error().message());
                    return;
                }

                if(fanmode.value() == FanMode::Unknown)
                {
                    emit errorOccurred(server_->lastError().value());
                    return;
                }

                if(coolerboost.isError())
                {
                    emit errorOccurred(res.error().message());
                    return;
                }

                if(coolerboost.value() == CoolerBoost::Unknown)
                {
                    emit errorOccurred(server_->lastError().value());
                    return;
                }

                if(coolerBoost_ != coolerboost.value())
                {
                    coolerBoost_ = coolerboost.value();

                    coolerBoostChanged();
                }
                setFanMode(fanmode.value());
            });
            timer->start(1000);
        }
    });
}

} // namespace mlfc
