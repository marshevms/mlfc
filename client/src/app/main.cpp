#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QDebug>
#include <qdbusmetatype.h>

#include "client.h"
#include "enumerationstorage.h"
#include "cpu.h"
#include "gpu.h"
#include "fanmodemodel.h"



void RegisterMetaType()
{
    qmlRegisterType<mlfc::FanModeModel>("FanModeModel", 1, 0, "FanModeModel");
    qmlRegisterUncreatableMetaObject(mlfc::EnumerationStorage::staticMetaObject, "EnumerationStorage", 1, 0, "EnumerationStorage", "Access to enums & flags only");

    qDBusRegisterMetaType<mlfc::EnumerationStorage::CoolerBoost>();
    qDBusRegisterMetaType<mlfc::EnumerationStorage::FanMode>();
}


int main(int argc, char **argv)
{
    if (!QDBusConnection::systemBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    RegisterMetaType();

    mlfc::CPU cpu;
    mlfc::GPU gpu;

    mlfc::Client client;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("cpu"), &cpu);
    engine.rootContext()->setContextProperty(QStringLiteral("gpu"), &gpu);
    engine.rootContext()->setContextProperty(QStringLiteral("client"), &client);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if(engine.rootObjects().isEmpty())
        return -1;

    if(!client.start(&cpu, &gpu))
    {
        qDebug() << client.lastError();

        return -1;
    }

//    QDBusServiceWatcher serviceWatcher("com.github.mlfc.server", QDBusConnection::sessionBus(), QDBusServiceWatcher::WatchForRegistration);


    return app.exec();
}

