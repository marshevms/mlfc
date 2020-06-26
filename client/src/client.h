#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "interface.h"
#include "serverstates.h"

namespace mlfc
{

class CPU;
class GPU;

}

namespace mlfc
{

class Client : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.mlfc.client")
    Q_ENUMS(mlfc::ServerStates)
    Q_PROPERTY(mlfc::ServerStates serverState READ server_state NOTIFY serverStateChanged)
public:

    using iserver = com::github::mlfc::server;

    explicit Client(QObject *parent = nullptr);

    bool Start(CPU *cpu, GPU *gpu);

    QString last_error() const;
    void set_last_error(const QString &error);

    ServerStates server_state();

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

private:
    CPU *cpu_;
    GPU *gpu_;

    iserver *server;
    ServerStates server_state_;

    QString last_error_;

    void set_server_state(const ServerStates state);

};

} // namespace mlfc
#endif // CLIENT_H
