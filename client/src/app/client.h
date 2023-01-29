#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QPoint>
#include <QThread>

#include "serverinterface.h"
#include "enumerationstorage.h"
#include "model/model.h"
#include "model/qmltempsfanspeeds.h"

namespace mlfc
{

class CPU;
class GPU;

namespace config
{

class Config;

}

}

namespace mlfc
{

class Client : public QObject
{
    using Config = config::Config;
    using CpuGpu = model::CpuGpu;
    using QmlTempsFanSpeeds = model::qmlTempsFanSpeeds;

    using FanMode = mlfc::EnumerationStorage::FanMode;
    using ServerStates = mlfc::EnumerationStorage::ServerStates;
    using CoolerBoost = mlfc::EnumerationStorage::CoolerBoost;
    using ChartValues = mlfc::EnumerationStorage::ChartValues;
    using IconTheme = mlfc::EnumerationStorage::IconTheme;

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.mlfc.client")
    Q_PROPERTY(mlfc::EnumerationStorage::ServerStates serverState READ serverState NOTIFY serverStateChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::CoolerBoost coolerBoost READ coolerBoost NOTIFY coolerBoostChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::FanMode fanMode READ fanMode NOTIFY fanModeChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::ChartValues chartValues READ chartValues WRITE onSetChartValuesClicked NOTIFY chartValuesChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::IconTheme iconTheme READ iconTheme NOTIFY iconThemeChanged)
    Q_PROPERTY(QString srvVersion READ serverVersion NOTIFY serverVersionChanged)
    Q_PROPERTY(QString ecVersion READ ecVersion NOTIFY ecVersionChanged)

public:

    explicit Client(QObject *parent = nullptr);

    bool start(CPU *cpu, GPU *gpu);

    QString lastError() const;
    void setLastError(const QString &error);

    ServerStates serverState();
    CoolerBoost coolerBoost();
    FanMode fanMode();
    ChartValues chartValues();
    QString serverVersion();
    QString ecVersion();

    IconTheme iconTheme();


signals:
    void errorOccurred(QString error);
    void serverStateChanged();

    void coolerBoostChanged();
    void fanModeChanged();

    void chartValuesChanged();

    void serverVersionChanged();
    void ecVersionChanged();

    void iconThemeChanged();

public slots:
    void init();

    void onSetCoolerBoostClicked(const mlfc::EnumerationStorage::CoolerBoost coolerBoost);
    void onSetFanModeClicked(const mlfc::EnumerationStorage::FanMode fanMode);
    void onSetChartValuesClicked(const mlfc::EnumerationStorage::ChartValues chartValues);

    void onSaveChartValuesClicked(const mlfc::model::qmlTempsFanSpeeds *tempsFanSpeeds, const mlfc::EnumerationStorage::ChartValues pu);

    void onSetIconThemeClicked(const mlfc::EnumerationStorage::IconTheme theme);

private:
    CPU *cpu_;
    GPU *gpu_;

    Config *config_;

    ServerInterface *server_;

    ServerStates serverState_;
    CoolerBoost coolerBoost_;
    FanMode fanMode_;
    ChartValues chartValues_;
    QString srvVersion_;
    QString ecVersion_;

    IconTheme iconTheme_;

    QString lastError_;

    QThread thread_;

    void setCoolerBoost(const mlfc::EnumerationStorage::CoolerBoost coolerBoost);
    void setFanMode(const mlfc::EnumerationStorage::FanMode fanMode);

    void setCpuTemp(int temp);
    void setCpuFanRmp(int rpm);

    void setGpuTemp(int temp);
    void setGpuFanRmp(int rpm);

    void setCpuTemps(const QVector<int> &temps);
    void setCpuFanSpeeds(const QVector<int> &fanSpeeds);

    void setGpuTemps(const QVector<int> &temps);
    void setGpuFanSpeeds(const QVector<int> &fanSpeeds);

    void setServerVersion(const QString &ecVersion);

    void setECVersion(const QString &ecVersion);

    QString serverLastError();

    //INIT Functions

    void startServer();

    void updateFanMode();
    void updateCoolerBoost();
    void updateCpuTemps();
    void updateCpuFanSpeeds();
    void updateGpuTemps();
    void updateGpuFanSpeeds();
    void updateServerVersion();
    void updateECVersion();

    void setServerState(const mlfc::EnumerationStorage::ServerStates state);

    //Config
    void readConfig();
    void saveConfig();

    CpuGpu getConfigValues(const FanMode mode, const QString &preset = "");
    bool setConfigValues(const CpuGpu &pair, const FanMode mode, const QString &preset = "");

    FanMode getConfigMode();
    bool setConfigMode(const FanMode mode);

    IconTheme getConfigIconTheme();
    bool setConfigIconTheme(const IconTheme theme);

    void initConfig();
    void checkConfig();
    void cmpAndChange(CpuGpu &pair);

};

} // namespace mlfc
#endif // CLIENT_H
