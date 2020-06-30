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
    void RealtimeCPUTemp(int temp);
    void RealtimeCPUFanRPM(int rpm);

    void RealtimeGPUTemp(int temp);
    void RealtimeGPUFanRPM(int rpm);

    void AnErrorOccured(QString error);

public slots:
    bool Start();

    core::FanMode FanMode();
    core::CoolerBoost CoolerBoost();

    bool SetCoolerBoost(core::CoolerBoost cooler_boost);

    bool SetUpdateInterval(int interval);
    int UpdateInterval() const;

    QString last_error() const;
};

} // namespace mlfc

#endif // SERVERADAPTER_H
