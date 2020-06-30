#include "client.h"

#include "cpu.h"
#include "gpu.h"

namespace mlfc
{

Client::Client(QObject *parent)
    : QObject(parent)
    , server_state_(mlfc::ServerStates::None)
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

    server = new iserver("com.github.mlfc.server", "/Server", QDBusConnection::systemBus(), this);

    connect(server, &iserver::RealtimeCPUTemp, this, &Client::set_cpu_temp);
    connect(server, &iserver::RealtimeCPUFanRPM, this, &Client::set_cpu_fan_rmp);

    connect(server, &iserver::RealtimeGPUTemp, this, &Client::set_gpu_temp);
    connect(server, &iserver::RealtimeGPUFanRPM, this, &Client::set_gpu_fan_rmp);

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

mlfc::ServerStates Client::server_state()
{
    return server_state_;
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

void Client::set_server_state(const mlfc::ServerStates state)
{
    server_state_ = state;
    emit serverStateChanged();
}

void Client::tryStartServer()
{
    auto res = server->Start();

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
            emit errorOccurred(server->last_error().value());
        }
        else
        {
            set_server_state(ServerStates::Working);
        }
    });
}

} // namespace mlfc