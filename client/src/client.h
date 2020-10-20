#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QPoint>

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
    using ChartValues = mlfc::EnumerationStorage::ChartValues;

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.mlfc.client")
    Q_PROPERTY(mlfc::EnumerationStorage::ServerStates serverState READ serverState NOTIFY serverStateChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::CoolerBoost coolerBoost READ coolerBoost NOTIFY coolerBoostChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::FanMode fanMode READ fanMode NOTIFY fanModeChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::ChartValues chartValues READ chartValues WRITE setChartValues NOTIFY chartValuesChanged)
public:


    explicit Client(QObject *parent = nullptr);

    bool start(CPU *cpu, GPU *gpu);

    QString lastError() const;
    void setLastError(const QString &error);

    ServerStates serverState();
    CoolerBoost coolerBoost();
    FanMode fanMode();
    ChartValues chartValues();

signals:
    void errorOccurred(QString error);
    void serverStateChanged();

    void coolerBoostChanged();
    void fanModeChanged();

    void chartValuesChanged();

public slots:
    void init();

    void setCoollerBoost(const mlfc::EnumerationStorage::CoolerBoost coolerBoost);
    void setFanMode(const mlfc::EnumerationStorage::FanMode fanMode);
    void setChartValues(const mlfc::EnumerationStorage::ChartValues chartValues);

    void saveChartValues(const QVector<QPoint> &values);

private:
    CPU *cpu_;
    GPU *gpu_;

    ServerInterface *server_;

    ServerStates serverState_;
    CoolerBoost coolerBoost_;
    FanMode fanMode_;
    ChartValues chartValues_;

    QString lastError_;

    void setCpuTemp(int temp);
    void setCpuFanRmp(int rpm);

    void setGpuTemp(int temp);
    void setGpuFanRmp(int rpm);

    void setCpuTemps(const QVector<int> &temps);
    void setCpuFanSpeeds(const QVector<int> &fanSpeeds);

    void setGpuTemps(const QVector<int> &temps);
    void setGpuFanSpeeds(const QVector<int> &fanSpeeds);

    QString serverLastError();

    //INIT Functions

    void startServer();

    void updateFanMode();
    void updateCoolerBoost();
    void updateCpuTemps();
    void updateCpuFanSpeeds();
    void updateGpuTemps();
    void updateGpuFanSpeeds();

    void setServerState(const mlfc::EnumerationStorage::ServerStates state);

};

} // namespace mlfc
#endif // CLIENT_H
