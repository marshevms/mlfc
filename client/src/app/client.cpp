#include "client.h"

#include <QMetaEnum>
#include <QTimer>
#include <QStandardPaths>
#include <QFile>
#include <QDebug>

#include "cpu.h"
#include "gpu.h"
#include "config.h"
#include "constants.h"

namespace mlfc
{

Client::Client(QObject *parent)
    : QObject(parent)
    , config_(new Config(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation).toStdString() + "/mlfc.toml"))
    , serverState_(ServerStates::Unknown)
    , coolerBoost_(CoolerBoost::Unknown)
    , fanMode_(FanMode::Unknown)
    , chartValues_(ChartValues::CPU)
    , iconTheme_(IconTheme::Unknown)
{
}

bool Client::start(CPU *cpu, GPU *gpu)
{
    if(!QDBusConnection::systemBus().registerService("com.github.mlfc.client"))
    {
        setLastError("\"com.github.mlfc.client\" already exists");
        return false;
    }
    if (!QDBusConnection::systemBus().registerObject("/Client", this)){
        setLastError("object \"/Client\" already exists");
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

Client::IconTheme Client::iconTheme()
{
    return iconTheme_;
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
    setFanMode(fanMode);
    setConfigMode(fanMode);

    auto pair = getConfigValues(fanMode);
    cmpAndChange(pair);
}

void Client::onSetChartValuesClicked(const EnumerationStorage::ChartValues chartValues)
{
    chartValues_ = chartValues;
    emit chartValuesChanged();
}

// I don't know why I can't use const model::qmlTempsFanSpeeds&
void Client::onSaveChartValuesClicked(const QmlTempsFanSpeeds *tempsFanSpeeds, const ChartValues pu)
{
    assert(tempsFanSpeeds && "type is nullptr");

    auto &temps = tempsFanSpeeds->tempsFanSpeeds.temps;
    auto &fanSpeeds = tempsFanSpeeds->tempsFanSpeeds.fanSpeeds;

    switch (pu)
    {
    case ChartValues::CPU:
    {
        qDebug() << "INFO: Trying to change cpu values";

        auto resTemps = server_->setCPUTemps(model::toQVector(temps));
        auto resFanSpeeds = server_->setCPUFanSpeeds(model::toQVector(fanSpeeds));

        cpu_->setTemps(model::toQVector(temps));
        cpu_->setFanSpeeds(model::toQVector(fanSpeeds));

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

        if (!config_->setCpuTemps(temps, fanMode_) || !config_->setCpuFanSpeeds(fanSpeeds, fanMode_))
        {
            emit errorOccurred(("Can't set CPU values"));
        }

        saveConfig();

        break;
    }
    case ChartValues::GPU:
    {
        qDebug() << "INFO: Trying to change gpu values";

        auto resTemps = server_->setGPUTemps(model::toQVector(temps));
        auto resFanSpeeds = server_->setGPUFanSpeeds(model::toQVector(fanSpeeds));

        gpu_->setTemps(model::toQVector(temps));
        gpu_->setFanSpeeds(model::toQVector(fanSpeeds));

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

        if (!config_->setGpuTemps(temps, fanMode_) || !config_->setGpuFanSpeeds(fanSpeeds, fanMode_))
        {
            emit errorOccurred(("Can't set GPU values"));
        }

        saveConfig();

        break;
    }
    }
}

void Client::onSetIconThemeClicked(const EnumerationStorage::IconTheme theme)
{
    iconTheme_ = theme;
    setConfigIconTheme(theme);
    emit iconThemeChanged();
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

        checkConfig();
    });

    oneSecTimer->start(1000);
    fiveSecTimer->start(5000);

    initConfig();
    checkConfig();
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

void Client::readConfig()
{
    if(!config_->read())
    {
        setLastError(QString::fromStdString(config_->lastError()));
        return emit errorOccurred(lastError());
    }

    if(!QFile::exists(QString::fromStdString(config_->getFilePath())))
    {
        return saveConfig();
    }
}

void Client::saveConfig()
{
    if (!config_->save())
    {
        setLastError(QString::fromStdString(config_->lastError()));
        return emit errorOccurred(lastError());
    }
}

Client::CpuGpu Client::getConfigValues(const Client::FanMode mode, const QString &preset)
{
    CpuGpu result;

    result.cpu.temps = config_->getCpuTemps(mode, preset.toStdString()).value_or(std::vector<int>{});
    result.cpu.fanSpeeds = config_->getCpuFanSpeeds(mode, preset.toStdString()).value_or(std::vector<int>{});
    result.gpu.temps = config_->getGpuTemps(mode, preset.toStdString()).value_or(std::vector<int>{});
    result.gpu.fanSpeeds = config_->getGpuFanSpeeds(mode, preset.toStdString()).value_or(std::vector<int>{});

    return result;
}

bool Client::setConfigValues(const Client::CpuGpu &pair, const Client::FanMode mode, const QString &preset)
{
    bool ok = true;

    ok &= config_->setCpuTemps(pair.cpu.temps, mode, preset.toStdString());
    ok &= config_->setCpuFanSpeeds(pair.cpu.fanSpeeds, mode, preset.toStdString());
    ok &= config_->setGpuTemps(pair.gpu.temps, mode, preset.toStdString());
    ok &= config_->setGpuFanSpeeds(pair.gpu.fanSpeeds, mode, preset.toStdString());

    return ok;
}

Client::FanMode Client::getConfigMode()
{
    return config_->getCurrentMode();
}

bool Client::setConfigMode(const FanMode mode)
{
    if (config_->getCurrentMode() != mode)
    {
        return  config_->setCurrentMode(mode) && config_->save();
    }

    return true;
}

Client::IconTheme Client::getConfigIconTheme()
{
    return config_->getIconTheme();
}

bool Client::setConfigIconTheme(const Client::IconTheme theme)
{
    if(config_->getIconTheme() != theme)
    {
        return config_->setIconTheme(theme) && config_->save();
    }

    return true;
}


void Client::initConfig()
{
    readConfig();

    auto isConfigEdit = false;

    if (fanMode_ == FanMode::Auto && model::isEmpty(getConfigValues(FanMode::Auto)))
    {
        const auto cpuTemps = cpu_->temps();
        const auto cpuFanSpeeds = cpu_->fanSpeeds();
        const auto gpuTemps = gpu_->temps();
        const auto gpuFanSpeeds = gpu_->fanSpeeds();

        CpuGpu pair{{{cpuTemps.begin(), cpuTemps.end()}, {cpuFanSpeeds.begin(), cpuFanSpeeds.end()}}
                    ,{{gpuTemps.begin(), gpuTemps.end()}, {gpuFanSpeeds.begin(), gpuFanSpeeds.end()}}};

        if (!setConfigValues(pair, FanMode::Auto, ""))
        {
            setLastError(QString::fromStdString(config_->lastError()));
            emit errorOccurred(lastError());
        }
        else
        {
            isConfigEdit = true;
        }
    } else if (fanMode_ != FanMode::Auto && model::isEmpty(getConfigValues(FanMode::Auto)))
    {
        setLastError(QString("please fill the config file: %1").arg(QString::fromStdString(config_->getFilePath())));
        emit errorOccurred(lastError());
    }

    if (model::isEmpty(getConfigValues(FanMode::Advanced)))
    {
        auto pair = getConfigValues(FanMode::Auto);
        if (!model::isEmpty(pair))
        {
            if (!setConfigValues(pair, FanMode::Advanced))
            {
                setLastError(QString::fromStdString(config_->lastError()));
                emit errorOccurred(lastError());
            }
            else
            {
                isConfigEdit = true;
            }
        }
    }

    if (isConfigEdit && !config_->save())
    {
        setLastError(QString("can't save config file: %1").arg(QString::fromStdString(config_->lastError())));
        emit errorOccurred(lastError());
    }

    return;
}

void Client::checkConfig()
{
    auto mode = getConfigMode();
    auto theme = getConfigIconTheme();
    auto pair = getConfigValues(mode);

    if (model::isEmpty(pair))
    {
        setLastError(QString("please fill the config file: %1").arg(QString::fromStdString(config_->getFilePath())));
        emit errorOccurred(lastError());
        return;
    }

    if (mode != fanMode_) onSetFanModeClicked(mode);
    if (theme != iconTheme_) onSetIconThemeClicked(theme);

    cmpAndChange(pair);
}

void Client::cmpAndChange(Client::CpuGpu &pair)
{
     auto tempsFanSpeeds = std::make_unique<QmlTempsFanSpeeds>();

     //CPU
     {
         auto temps = cpu_->rTemps();
         auto fanSpeeds = cpu_->rFanSpeeds();
         model::TempsFanSpeeds cpuPair{{temps.begin(), temps.end()}, {fanSpeeds.begin(), fanSpeeds.end()}};

         if (!pair.cpu.isEqual(cpuPair))
         {
             tempsFanSpeeds->tempsFanSpeeds = std::move(pair.cpu);
             onSaveChartValuesClicked(tempsFanSpeeds.get(),  ChartValues::CPU);
         }
     }

     //GPU
     {
         auto temps = gpu_->rTemps();
         auto fanSpeeds = gpu_->rFanSpeeds();;
         model::TempsFanSpeeds gpuPair{{temps.begin(), temps.end()}, {fanSpeeds.begin(), fanSpeeds.end()}};

         if (!pair.gpu.isEqual(gpuPair))
         {
             tempsFanSpeeds->tempsFanSpeeds = std::move(pair.gpu);
             onSaveChartValuesClicked(tempsFanSpeeds.get(),  ChartValues::GPU);
         }
     }
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
