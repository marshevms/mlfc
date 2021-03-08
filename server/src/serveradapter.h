#ifndef SERVERADAPTER_H
#define SERVERADAPTER_H

#include <QtDBus/QDBusAbstractAdaptor>
#include <QDBusArgument>

#include "core/constants.h"
#include "utils/utils.h"

namespace mlfc
{

class ServerAdapter: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.mlfc.server")
public:
    ServerAdapter(QObject *parent);

signals:
    void realtimeCPUTemp(int temp);
    void realtimeCPUFanRPM(int rpm);

    void realtimeGPUTemp(int temp);
    void realtimeGPUFanRPM(int rpm);

    void anErrorOccured(QString error);

public slots:
    bool start();

    core::FanMode fanMode();
    core::CoolerBoost coolerBoost();

    bool setCoolerBoost(core::CoolerBoost cooler_boost);
    bool setFanMode(core::FanMode fan_mode);

    QVector<int> cpuTemps();
    QVector<int> cpuFanSpeeds();

    QVector<int> gpuTemps();
    QVector<int> gpuFanSpeeds();

    bool setCPUTemps(QVector<int> cpuTemps);
    bool setCPUFanSpeeds(QVector<int> cpuFanSpeeds);

    bool setGPUTemps(QVector<int> gpuTemps);
    bool setGPUFanSpeeds(QVector<int> gpuFanSpeeds);

    QString ecVersion();

    bool setUpdateInterval(int interval);
    int updateInterval() const;

    QString lastError() const;
};

} // namespace mlfc

#endif // SERVERADAPTER_H
