#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QObject>
#include <QDBusAbstractInterface>
#include <QDBusPendingReply>

#include "enumerationstorage.h"
#include "utils.h"


namespace mlfc
{

class ServerInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    ServerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);
    ~ServerInterface() = default;

    static inline const char *staticInterfaceName()
    { return "com.github.mlfc.server"; }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<EnumerationStorage::CoolerBoost> CoolerBoost()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CoolerBoost"), argumentList);
    }

    inline QDBusPendingReply<EnumerationStorage::FanMode> FanMode()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("FanMode"), argumentList);
    }

    inline QDBusPendingReply<bool> SetCoolerBoost(EnumerationStorage::CoolerBoost cooler_boost)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(cooler_boost);
        return asyncCallWithArgumentList(QStringLiteral("SetCoolerBoost"), argumentList);
    }

    inline QDBusPendingReply<bool> SetFanMode(EnumerationStorage::FanMode fan_mode)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(fan_mode);
        return asyncCallWithArgumentList(QStringLiteral("SetFanMode"), argumentList);
    }

    inline QDBusPendingReply<bool> SetUpdateInterval(int interval)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(interval);
        return asyncCallWithArgumentList(QStringLiteral("SetUpdateInterval"), argumentList);
    }

    inline QDBusPendingReply<bool> Start()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Start"), argumentList);
    }

    inline QDBusPendingReply<int> UpdateInterval()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("UpdateInterval"), argumentList);
    }

    inline QDBusPendingReply<QString> last_error()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("last_error"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void AnErrorOccured(const QString &error);
    void RealtimeCPUFanRPM(int rpm);
    void RealtimeCPUTemp(int temp);
    void RealtimeGPUFanRPM(int rpm);
    void RealtimeGPUTemp(int temp);
};

}

#endif // SERVERINTERFACE_H
