#include "client.h"

#include <QTimer>
#include <QDebug>
#include <QMetaEnum>

#include "cpu.h"
#include "gpu.h"
#include "constants.h"

namespace mlfc
{

Client::Client(QObject *parent)
    : QObject(parent)
    , serverState_(ServerStates::Unknown)
    , coolerBoost_(CoolerBoost::Unknown)
    , fanMode_(FanMode::Unknown)
    , chartValues_(ChartValues::CPU)
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

    connect(server_, &ServerInterface::realtimeCPUTemp, this, &Client::setCpuTemp);
    connect(server_, &ServerInterface::realtimeCPUFanRPM, this, &Client::setCpuFanRmp);

    connect(server_, &ServerInterface::realtimeGPUTemp, this, &Client::setGpuTemp);
    connect(server_, &ServerInterface::realtimeGPUFanRPM, this, &Client::setGpuFanRmp);

    init();
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

Client::ChartValues Client::chartValues()
{
    return chartValues_;
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

void Client::onSetCoolerBoostClicked(const mlfc::EnumerationStorage::CoolerBoost coolerBoost)
{
    auto res = server_->setCoolerBoost(coolerBoost);
    res.waitForFinished();

    if(res.isError())
    {
        setCoolerBoost(CoolerBoost::Unknown);
        emit errorOccurred(res.error().message());
        return;
    }

    if(!res.value())
    {
        setCoolerBoost(CoolerBoost::Unknown);
        emit errorOccurred(serverLastError());
        return;
    }
}

void Client::onSetFanModeClicked(const mlfc::EnumerationStorage::FanMode fanMode)
{
    auto res = server_->setFanMode(fanMode);
    res.waitForFinished();

    if(res.isError())
    {
        setFanMode(FanMode::Unknown);
        emit errorOccurred(res.error().message());
        return;
    }

    if(!res.value())
    {
        setFanMode(FanMode::Unknown);
        emit errorOccurred(serverLastError());
        return;
    }

}

void Client::onSetChartValuesClicked(const EnumerationStorage::ChartValues chartValues)
{
    chartValues_ = chartValues;
    emit chartValuesChanged();
}

void Client::onSaveChartValuesClicked(const QVector<QPoint> &values)
{
    QVector<int> temps;
    QVector<int> fanSpeeds;

    temps.reserve(kTempsNumber);
    fanSpeeds.reserve(kFanSpeedsNumber);

    std::transform(values.begin() + 1, values.end(), std::back_inserter(temps), [](const QPoint &point){
        return point.x();
    });

    std::transform(values.begin(), values.end(), std::back_inserter(fanSpeeds), [](const QPoint &point){
        return point.y();
    });

    switch (chartValues_)
    {
    case ChartValues::CPU:
    {
        qDebug() << "INFO: Trying to change cpu values";

        auto resTemps = server_->setCPUTemps(temps);
        auto resFanSpeeds = server_->setCPUFanSpeeds(fanSpeeds);

        cpu_->setTemps(temps);
        cpu_->setFanSpeeds(fanSpeeds);

        auto value = resTemps.value();
        if(resTemps.isError())
        {
            emit errorOccurred(resTemps.error().message());
            return;
        }

        if(!value)
        {
            emit errorOccurred(serverLastError());
            return;
        }

        value = resFanSpeeds.value();
        if(resFanSpeeds.isError())
        {
            emit errorOccurred(resFanSpeeds.error().message());
            return;
        }

        if(!value)
        {
            emit errorOccurred(serverLastError());
            return;
        }

        break;
    }
    case ChartValues::GPU:
    {
        qDebug() << "INFO: Trying to change gpu values";

        auto resTemps = server_->setGPUTemps(temps);
        auto resFanSpeeds = server_->setGPUFanSpeeds(fanSpeeds);

        gpu_->setTemps(temps);
        gpu_->setFanSpeeds(fanSpeeds);

        auto value = resTemps.value();
        if(resTemps.isError())
        {
            emit errorOccurred(resTemps.error().message());
            return;
        }

        if(!value)
        {
            emit errorOccurred(serverLastError());
            return;
        }

        value = resFanSpeeds.value();
        if(resFanSpeeds.isError())
        {
            emit errorOccurred(resFanSpeeds.error().message());
            return;
        }

        if(!value)
        {
            emit errorOccurred(serverLastError());
            return;
        }

        break;
    }
    }
}

void Client::setCoolerBoost(const EnumerationStorage::CoolerBoost coolerBoost)
{
    coolerBoost_ = coolerBoost;
    emit coolerBoostChanged();
}

void Client::setFanMode(const EnumerationStorage::FanMode fanMode)
{
    fanMode_ = fanMode;
    emit fanModeChanged();
}

void Client::init()
{
    startServer();

    if (ServerStates::Working != serverState())
        return;

    updateFanMode();
    updateCoolerBoost();

    updateCpuTemps();
    updateCpuFanSpeeds();

    updateGpuTemps();
    updateGpuFanSpeeds();

    QTimer *oneSecTimer = new QTimer(this);
    QTimer *fiveSecTimer = new QTimer(this);

    connect(oneSecTimer, &QTimer::timeout, this, [=]{
        updateFanMode();
        updateCoolerBoost();
    });
    connect(fiveSecTimer, &QTimer::timeout, this, [=]{
        updateCpuTemps();
        updateCpuFanSpeeds();

        updateGpuTemps();
        updateGpuFanSpeeds();
    });

    oneSecTimer->start(1000);
    fiveSecTimer->start(5000);
}

void Client::startServer()
{
    setServerState(ServerStates::Starting);

    auto res = server_->start();

    auto value = res.value();
    if (res.isError())
    {
        setServerState(ServerStates::Stopped);
        emit errorOccurred(res.error().message());
        return;
    }
    if (!value)
    {
        setServerState(ServerStates::Stopped);
        emit errorOccurred(serverLastError());
        return;
    }

    return setServerState(ServerStates::Working);
}

void Client::updateFanMode()
{
    auto fanMode = server_->fanMode();

    auto value = fanMode.value();
    if (fanMode.isError())
    {
        emit errorOccurred(fanMode.error().message());
        return;
    }

    if (FanMode::Unknown == value)
    {
        emit errorOccurred(serverLastError());
        return;
    }

    setFanMode(value);

    return;
}

void Client::updateCoolerBoost()
{
    auto coolerBoost = server_->coolerBoost();

    auto value = coolerBoost.value();
    if (coolerBoost.isError())
    {
        emit errorOccurred(coolerBoost.error().message());
        return;
    }

    if (CoolerBoost::Unknown == value)
    {
        emit errorOccurred(serverLastError());
        return;
    }

    setCoolerBoost(value);

    return;
}

void Client::updateCpuTemps()
{
    auto cpuTemps = server_->cpuTemps();

    auto value = cpuTemps.value();
    if(cpuTemps.isError())
    {
        emit errorOccurred(cpuTemps.error().message());
        return;
    }

    return setCpuTemps(value);
}

void Client::updateCpuFanSpeeds()
{
    auto cpuFanSpeeds = server_->cpuFanSpeeds();

    auto value = cpuFanSpeeds.value();
    if(cpuFanSpeeds.isError())
    {
        emit errorOccurred(cpuFanSpeeds.error().message());
        return;
    }

    return setCpuFanSpeeds(value);
}

void Client::updateGpuTemps()
{
    auto gpuTemps = server_->gpuTemps();

    auto value = gpuTemps.value();
    if(gpuTemps.isError())
    {
        emit errorOccurred(gpuTemps.error().message());
        return;
    }

    return setGpuTemps(value);
}

void Client::updateGpuFanSpeeds()
{
    auto gpuFanSpeeds = server_->gpuFanSpeeds();

    auto value = gpuFanSpeeds.value();
    if(gpuFanSpeeds.isError())
    {
        emit errorOccurred(gpuFanSpeeds.error().message());
        return;
    }

    return setGpuFanSpeeds(value);
}


void Client::setServerState(const mlfc::EnumerationStorage::ServerStates state)
{
    serverState_ = state;
    emit serverStateChanged();
}

void Client::setCpuTemps(const QVector<int> &temps)
{
    if (cpu_->setTemps(temps) < 0)
    {
        qDebug() << "ERROR: Can't set CPU temps";
    }
}

void Client::setCpuFanSpeeds(const QVector<int> &fanSpeeds)
{
    if (cpu_->setFanSpeeds(fanSpeeds) < 0)
    {
        qDebug() << "ERROR: Can't set CPU fan speeds";
    }
}

void Client::setGpuTemps(const QVector<int> &temps)
{
    if (gpu_->setTemps(temps) < 0)
    {
        qDebug() << "ERROR: Can't set GPU temps";
    }
}

void Client::setGpuFanSpeeds(const QVector<int> &fanSpeeds)
{
    if (gpu_->setFanSpeeds(fanSpeeds) < 0)
    {
        qDebug() << "ERROR: Can't set GPU fan speeds";
    }
}

QString Client::serverLastError()
{
    auto error = server_->lastError();

    auto value = error.value();
    if (error.isError())
    {
        return error.error().message();
    }

    return value;
}

} // namespace mlfc
