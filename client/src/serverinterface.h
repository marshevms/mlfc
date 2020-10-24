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
    inline QDBusPendingReply<EnumerationStorage::CoolerBoost> coolerBoost()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("coolerBoost"), argumentList);
    }

    inline QDBusPendingReply<EnumerationStorage::FanMode> fanMode()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("fanMode"), argumentList);
    }

    inline QDBusPendingReply<bool> setCoolerBoost(EnumerationStorage::CoolerBoost cooler_boost)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(cooler_boost);
        return asyncCallWithArgumentList(QStringLiteral("setCoolerBoost"), argumentList);
    }

    inline QDBusPendingReply<bool> setFanMode(EnumerationStorage::FanMode fan_mode)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(fan_mode);
        return asyncCallWithArgumentList(QStringLiteral("setFanMode"), argumentList);
    }

    inline QDBusPendingReply<bool> setUpdateInterval(int interval)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(interval);
        return asyncCallWithArgumentList(QStringLiteral("setUpdateInterval"), argumentList);
    }

    inline QDBusPendingReply<QVector<int>> cpuTemps()
    {
        return asyncCallWithArgumentList(QStringLiteral("cpuTemps"), {});
    }

    inline QDBusPendingReply<QVector<int>> cpuFanSpeeds()
    {
        return asyncCallWithArgumentList(QStringLiteral("cpuFanSpeeds"), {});
    }

    inline QDBusPendingReply<QVector<int>> gpuTemps()
    {
        return asyncCallWithArgumentList(QStringLiteral("gpuTemps"), {});
    }

    inline QDBusPendingReply<QVector<int>> gpuFanSpeeds()
    {
        return asyncCallWithArgumentList(QStringLiteral("gpuFanSpeeds"), {});
    }

    inline QDBusPendingReply<bool> start()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("start"), argumentList);
    }

    inline QDBusPendingReply<int> updateInterval()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("updateInterval"), argumentList);
    }

    inline QDBusPendingReply<QString> lastError()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("lastError"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void anErrorOccured(const QString &error);
    void realtimeCPUFanRPM(int rpm);
    void realtimeCPUTemp(int temp);
    void realtimeGPUFanRPM(int rpm);
    void realtimeGPUTemp(int temp);
};

}

#endif // SERVERINTERFACE_H
