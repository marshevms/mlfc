#include "server.h"

#include <QTimer>

#include "../server_info.h"
#include "core/manager.h"
#include "serveradapter.h"

namespace mlfc {

Server::Server(QObject* parent)
    : QObject(parent)
    , server_status_(ServerStatus::kOFF)
    , manager_(new mlfc::core::Manager())
{
}

Server::~Server() { }

int Server::realtimeCPUTemp() {
    try {
         return manager_->realtimeCPUTemp();
     } catch (std::runtime_error& e) {
         setLastError(e.what());
         emit anErrorOccured(lastError());
     }

     return 0;
 }

int Server::realtimeCPUFanRPM() { 
    try {
         return manager_->realtimeCPUFanRPM();
     } catch (std::runtime_error& e) {
         setLastError(e.what());
         emit anErrorOccured(lastError());
     }

     return 0;
}

int Server::realtimeGPUTemp() {
    try {
         return manager_->realtimeGPUTemp();
     } catch (std::runtime_error& e) {
         setLastError(e.what());
         emit anErrorOccured(lastError());
     }

     return 0;
}

int Server::realtimeGPUFanRPM() { 
    try {
         return manager_->realtimeGPUFanRPM();
     } catch (std::runtime_error& e) {
         setLastError(e.what());
         emit anErrorOccured(lastError());
     }

     return 0;
}

core::FanMode Server::fanMode()
{
   try {
        return manager_->fanMode();
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return core::FanMode::Unknown;
}

core::CoolerBoost Server::coolerBoost()
{
    try {
        return manager_->coolerBoost();
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return core::CoolerBoost::Unknown;
}

bool Server::setCoolerBoost(core::CoolerBoost cooler_boost)
{
    try {
        manager_->setCoolerBoost(cooler_boost);

        return true;
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setFanMode(core::FanMode fan_mode)
{
    try {
        manager_->setFanMode(fan_mode);

        return true;
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

QVector<int> Server::cpuTemps()
{
    try {
        auto cpuTemps = manager_->cpuTemps();
        return QVector<int>(cpuTemps.begin(), cpuTemps.end());
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

QVector<int> Server::cpuFanSpeeds()
{
    try {
        auto cpuFanSpeeds = manager_->cpuFanSpeeds();
        return QVector<int>(cpuFanSpeeds.begin(), cpuFanSpeeds.end());
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

QVector<int> Server::gpuTemps()
{
    try {
        auto gpuTemps = manager_->gpuTemps();
        return QVector<int>(gpuTemps.begin(), gpuTemps.end());
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

QVector<int> Server::gpuFanSpeeds()
{
    try {
        auto fanSpeeds = manager_->gpuFanSpeeds();
        return QVector<int>(fanSpeeds.begin(), fanSpeeds.end());
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

bool Server::setCPUTemps(QVector<int>& cpuTemps)
{
    try {
        manager_->setCPUTemps({ cpuTemps.begin(), cpuTemps.end() });
        return true;
    } catch (std::logic_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setCPUFanSpeeds(QVector<int>& cpuFanSpeeds)
{
    try {
        manager_->setCPUFanSpeeds({ cpuFanSpeeds.begin(), cpuFanSpeeds.end() });
        return true;
    } catch (std::logic_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setGPUTemps(QVector<int>& gpuTemps)
{
    try {
        manager_->setGPUTemps({ gpuTemps.begin(), gpuTemps.end() });
        return true;
    } catch (std::logic_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

bool Server::setGPUFanSpeeds(QVector<int>& gpuFanSpeeds)
{
    try {
        manager_->setGPUFanSpeeds({ gpuFanSpeeds.begin(), gpuFanSpeeds.end() });
        return true;
    } catch (std::logic_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return false;
}

QString Server::ecVersion()
{
    try {
        auto ec = manager_->ecVersion();
        return QString::fromStdString(ec);
    } catch (std::runtime_error& e) {
        setLastError(e.what());
        emit anErrorOccured(lastError());
    }

    return {};
}

QString Server::serverVersion()
{
    return PROJECT_VER;
}

QString Server::lastError() const
{
    return last_error_;
}

void Server::setLastError(const QString& error)
{
    last_error_ = error;
}

} // namespace mlfc
