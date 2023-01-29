#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QDBusError>
#include <QDebug>
#include <iostream>
#include <qdbusmetatype.h>

#include "server.h"
#include "serveradapter.h"

Q_DECLARE_METATYPE(mlfc::core::CoolerBoost)
Q_DECLARE_METATYPE(mlfc::core::FanMode)

void RegisterMetaType()
{
    qRegisterMetaType<mlfc::core::CoolerBoost>("core::CoolerBoost");
    qRegisterMetaType<mlfc::core::FanMode>("core::FanMode");

    qDBusRegisterMetaType<mlfc::core::CoolerBoost>();
    qDBusRegisterMetaType<mlfc::core::FanMode>();
}

int main(int argc, char **argv)
{
        QCoreApplication app(argc, argv);

        QDBusServiceWatcher service_watcher("com.github.mlfc.client", QDBusConnection::systemBus()
                                           , QDBusServiceWatcher::WatchForUnregistration);

        QObject::connect(&service_watcher, &QDBusServiceWatcher::serviceUnregistered
                         , &app, &QCoreApplication::quit);

        RegisterMetaType();

        mlfc::Server server;

        auto *serverAdapter = new mlfc::ServerAdapter(&server);

        std::cout << int(serverAdapter->coolerBoost());

        if(!QDBusConnection::systemBus().registerService("com.github.mlfc.server")
           || !QDBusConnection::systemBus().registerObject("/Server", &server))
        {
            qDebug() << QDBusConnection::systemBus().lastError().message();
        }

        return app.exec();
}
