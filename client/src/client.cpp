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
    , server_state_(ServerStates::Unknown)
    , cooler_boost_(CoolerBoost::Unknown)
    , fan_mode_(FanMode::Unknown)
{
}

bool Client::Start(CPU *cpu, GPU *gpu)
{
    if(!QDBusConnection::systemBus().registerService("com.github.mlfc.client")
       || !QDBusConnection::systemBus().registerObject("/Client", this))
    {
        set_last_error(QDBusConnection::systemBus().lastError().message());

        return false;
    }

    if(cpu == nullptr || gpu == nullptr)
        return false;

    cpu_ = cpu;
    gpu_ = gpu;

    server_ = new ServerInterface("com.github.mlfc.server", "/Server", QDBusConnection::systemBus(), this);

    connect(server_, &ServerInterface::RealtimeCPUTemp, this, &Client::set_cpu_temp);
    connect(server_, &ServerInterface::RealtimeCPUFanRPM, this, &Client::set_cpu_fan_rmp);

    connect(server_, &ServerInterface::RealtimeGPUTemp, this, &Client::set_gpu_temp);
    connect(server_, &ServerInterface::RealtimeGPUFanRPM, this, &Client::set_gpu_fan_rmp);

    tryStartServer();
    return true;
}

QString Client::last_error() const
{
    return last_error_;
}

void Client::set_last_error(const QString &error)
{
    last_error_ = error;
}

Client::ServerStates Client::server_state()
{
    return server_state_;
}

Client::CoolerBoost Client::cooler_boost()
{
    return cooler_boost_;
}

Client::FanMode Client::fan_mode()
{
    return fan_mode_;
}

void Client::set_cpu_temp(int temp)
{
    cpu_->set_temp(temp);
//    qDebug() << "cpu: " << temp;
}

void Client::set_cpu_fan_rmp(int rpm)
{
    cpu_->set_fan_rpm(rpm);
//    qDebug() << "cpu: " << rpm;
}

void Client::set_gpu_temp(int temp)
{
    gpu_->set_temp(temp);
//    qDebug() << "gpu: " << temp;
}

void Client::set_gpu_fan_rmp(int rpm)
{
    gpu_->set_fan_rpm(rpm);
    //    qDebug() << "gpu: " << rpm;
}

void Client::set_cooller_boost(const mlfc::EnumerationStorage::CoolerBoost cooler_boost)
{
    auto res = server_->SetCoolerBoost(cooler_boost);
    res.waitForFinished();

    if(res.isError())
    {
        emit errorOccurred(res.error().message());
        return;
    }

    if(!res.value())
    {
        emit errorOccurred(server_->last_error().value());
        return;
    }
}

void Client::set_fan_mode(const mlfc::EnumerationStorage::FanMode fan_mode)
{
    if(fan_mode_ != fan_mode)
    {
        fan_mode_ = fan_mode;

        fanModeChanged();
    }
}

void Client::set_fan_mode(const int fan_mode)
{
    auto res = server_->SetFanMode(static_cast<FanMode>(fan_mode));
    res.waitForFinished();

    if(res.isError())
    {
        emit errorOccurred(res.error().message());
        return;
    }

    if(!res.value())
    {
        emit errorOccurred(server_->last_error().value());
        return;
    }

    set_fan_mode(static_cast<FanMode>(fan_mode));
}

void Client::set_server_state(const mlfc::EnumerationStorage::ServerStates state)
{
    server_state_ = state;
    emit serverStateChanged();
}

void Client::tryStartServer()
{
    auto res = server_->Start();

    set_server_state(ServerStates::Starting);

    QTimer::singleShot(1000, this, [=](){

        if(res.isError())
        {
            set_server_state(ServerStates::Stopped);
            emit errorOccurred(res.error().message());
            return;
        }

        if(res.value() == false)
        {
            set_server_state(ServerStates::Stopped);
            emit errorOccurred(server_->last_error().value());
        }
        else
        {
            set_server_state(ServerStates::Working);

            QTimer *timer = new QTimer(this);

            connect(timer, &QTimer::timeout, this, [=]{

                auto fanmode = server_->FanMode();
                auto coolerboost = server_->CoolerBoost();

                fanmode.waitForFinished();
                coolerboost.waitForFinished();

                if(fanmode.isError())
                {
                    emit errorOccurred(res.error().message());
                    return;
                }

                if(fanmode.value() == FanMode::Unknown)
                {
                    emit errorOccurred(server_->last_error().value());
                    return;
                }

                if(coolerboost.isError())
                {
                    emit errorOccurred(res.error().message());
                    return;
                }

                if(coolerboost.value() == CoolerBoost::Unknown)
                {
                    emit errorOccurred(server_->last_error().value());
                    return;
                }

                if(cooler_boost_ != coolerboost.value())
                {
                    cooler_boost_ = coolerboost.value();

                    coolerBoostChanged();
                }
                set_fan_mode(fanmode.value());
            });
            timer->start(1000);
        }
    });
}

} // namespace mlfc
