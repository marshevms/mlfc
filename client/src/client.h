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
    Q_PROPERTY(mlfc::EnumerationStorage::ServerStates serverState READ server_state NOTIFY serverStateChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::CoolerBoost coolerBoost READ cooler_boost NOTIFY coolerBoostChanged)
    Q_PROPERTY(mlfc::EnumerationStorage::FanMode fanMode READ fan_mode NOTIFY fanModeChanged)
public:


    explicit Client(QObject *parent = nullptr);

    bool Start(CPU *cpu, GPU *gpu);

    QString last_error() const;
    void set_last_error(const QString &error);

    ServerStates server_state();
    CoolerBoost cooler_boost();
    FanMode fan_mode();

signals:
    /**
     * @brief errorOccurred
     * @param error
     *
     * Note: Note that type names must begin with an uppercase letter
     * in order to be declared as QML object types in a QML file
     */
    void errorOccurred(QString error);
    /**
     * @brief serverStateChanged
     *
     * Note: Note that type names must begin with an uppercase letter
     * in order to be declared as QML object types in a QML file
     */
    void serverStateChanged();

    void coolerBoostChanged();
    void fanModeChanged();

public slots:
    /**
     * @brief tryStartServer
     *
     * Note: Note that type names must begin with an uppercase letter
     * in order to be declared as QML object types in a QML file
     */
    void tryStartServer();

    void set_cpu_temp(int temp);
    void set_cpu_fan_rmp(int rpm);

    void set_gpu_temp(int temp);
    void set_gpu_fan_rmp(int rpm);

    void set_cooller_boost(const mlfc::EnumerationStorage::CoolerBoost cooler_boost);

    void set_fan_mode(const mlfc::EnumerationStorage::FanMode fan_mode);
    void set_fan_mode(const int fan_mode);

private:
    CPU *cpu_;
    GPU *gpu_;

    ServerInterface *server_;

    ServerStates server_state_;
    CoolerBoost cooler_boost_;
    FanMode fan_mode_;

    QString last_error_;

    void set_server_state(const mlfc::EnumerationStorage::ServerStates state);

};

} // namespace mlfc
#endif // CLIENT_H
