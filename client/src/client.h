#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "serverinterface.h"
#include "enumerationstorage.h"

namespace mlfc
{

class CPU;
class GPU;

}

namespace mlfc
{

class Client : public QObject
{
    using FanMode = mlfc::EnumerationStorage::FanMode;
    using ServerStates = mlfc::EnumerationStorage::ServerStates;
    using CoolerBoost = mlfc::EnumerationStorage::CoolerBoost;

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.mlfc.client")
    Q_PROPERTY(mlfc::EnumerationStorage::ServerStates serverState READ serverState NOTIFY serverStateChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::CoolerBoost coolerBoost READ coolerBoost NOTIFY coolerBoostChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::FanMode fanMode READ fanMode NOTIFY fanModeChanged)
public:


    explicit Client(QObject *parent = nullptr);

    bool start(CPU *cpu, GPU *gpu);

    QString lastError() const;
    void setLastError(const QString &error);

    ServerStates serverState();
    CoolerBoost coolerBoost();
    FanMode fanMode();

signals:
    void errorOccurred(QString error);
    void serverStateChanged();

    void coolerBoostChanged();
    void fanModeChanged();

public slots:
    void tryStartServer();

    void setCpuTemp(int temp);
    void setCpuFanRmp(int rpm);

    void setGpuTemp(int temp);
    void setGpuFanRmp(int rpm);

    void setCoollerBoost(const mlfc::EnumerationStorage::CoolerBoost coolerBoost);

    void setFanMode(const mlfc::EnumerationStorage::FanMode fanMode);
    void setFanMode(const int fanMode);

private:
    CPU *cpu_;
    GPU *gpu_;

    ServerInterface *server_;

    ServerStates serverState_;
    CoolerBoost coolerBoost_;
    FanMode fanMode_;

    QString lastError_;

    void setServerState(const mlfc::EnumerationStorage::ServerStates state);

};

} // namespace mlfc
#endif // CLIENT_H
