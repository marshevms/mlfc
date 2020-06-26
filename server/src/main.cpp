#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QDBusError>
#include <QDebug>

#include  <iostream>

#include "server.h"
#include "serveradapter.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QDBusServiceWatcher service_watcher("com.github.mlfc.client", QDBusConnection::systemBus()
                                       , QDBusServiceWatcher::WatchForUnregistration);

    QObject::connect(&service_watcher, &QDBusServiceWatcher::serviceUnregistered
                     , &app, &QCoreApplication::quit);


    mlfc::Server server;

    auto *serverAdapter = new mlfc::ServerAdapter(&server);

    if(!QDBusConnection::systemBus().registerService("com.github.mlfc.server")
       || !QDBusConnection::systemBus().registerObject("/Server", &server))
    {
        qDebug() << QDBusConnection::systemBus().lastError().message();
    }

    return app.exec();
}
