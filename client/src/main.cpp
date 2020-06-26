#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDBusConnection>
#include <QDBusServiceWatcher>

#include "client.h"
#include "serverstates.h"
#include "cpu.h"
#include "gpu.h"

int main(int argc, char **argv)
{
    if (!QDBusConnection::systemBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qRegisterMetaType<mlfc::ServerStates>("mlfc::ServerStates");
    qmlRegisterUncreatableMetaObject(mlfc::staticMetaObject, "ServerStates", 1, 0, "ServerStates", "Access to enums & flags only");

    mlfc::CPU cpu;
    mlfc::GPU gpu;

    mlfc::Client client;

    if(!client.Start(&cpu, &gpu))
    {
        qDebug() << client.last_error();

        return -1;
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("cpu"), &cpu);
    engine.rootContext()->setContextProperty(QStringLiteral("gpu"), &gpu);
    engine.rootContext()->setContextProperty(QStringLiteral("client"), &client);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if(engine.rootObjects().isEmpty())
        return -1;




//    QDBusServiceWatcher serviceWatcher("com.github.mlfc.server", QDBusConnection::sessionBus(), QDBusServiceWatcher::WatchForRegistration);


    return app.exec();
}

