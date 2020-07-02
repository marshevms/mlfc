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
    using FanMode = EnumerationStorage::FanMode;
    using ServerStates = EnumerationStorage::ServerStates;
    using CoolerBoost = EnumerationStorage::CoolerBoost;

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.mlfc.client")
    Q_PROPERTY(int serverState READ server_state NOTIFY serverStateChanged)
    Q_PROPERTY(int coolerBoost READ cooler_boost NOTIFY coolerBoostChanged)
    Q_PROPERTY(int fanMode READ fan_mode NOTIFY fanModeChanged)
public:


    explicit Client(QObject *parent = nullptr);

    bool Start(CPU *cpu, GPU *gpu);

    QString last_error() const;
    void set_last_error(const QString &error);

    int server_state();
    int cooler_boost();
    int fan_mode();

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

    void set_cooller_boost(const CoolerBoost cooler_boost);
    void set_cooller_boost(const int cooler_boost);

    void set_fan_mode(const FanMode fan_mode);
    void set_fan_mode(const int fan_mode);

private:
    CPU *cpu_;
    GPU *gpu_;

    ServerInterface *server_;

    EnumerationStorage::ServerStates server_state_;
    CoolerBoost cooler_boost_;
    FanMode fan_mode_;

    QString last_error_;

    void set_server_state(const ServerStates state);

};

} // namespace mlfc
#endif // CLIENT_H
