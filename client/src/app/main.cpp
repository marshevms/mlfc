#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <qdbusmetatype.h>

#include <QDebug>

#include "client.h"
#include "appinfo.h"
#include "enumerationstorage.h"
#include "cpu.h"
#include "gpu.h"
#include "fanmodemodel.h"
#include "model/qmltempsfanspeeds.h"

void RegisterMetaType()
{   
    qRegisterMetaType<mlfc::model::qmlTempsFanSpeeds*>("const mlfc::model::qmlTempsFanSpeeds*");

    qmlRegisterType<mlfc::FanModeModel>("FanModeModel", 1, 0, "FanModeModel");
    qmlRegisterUncreatableMetaObject(mlfc::EnumerationStorage::staticMetaObject, "EnumerationStorage", 1, 0, "EnumerationStorage", "Access to enums & flags only");

    qDBusRegisterMetaType<mlfc::EnumerationStorage::CoolerBoost>();
    qDBusRegisterMetaType<mlfc::EnumerationStorage::FanMode>();
    qDBusRegisterMetaType<mlfc::EnumerationStorage::IconTheme>();
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
    mlfc::AppInfo appInfo;

    mlfc::model::qmlTempsFanSpeeds tempsFanSpeeds;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("cpu"), &cpu);
    engine.rootContext()->setContextProperty(QStringLiteral("gpu"), &gpu);
    engine.rootContext()->setContextProperty(QStringLiteral("client"), &client);
    engine.rootContext()->setContextProperty(QStringLiteral("appInfo"), &appInfo);
    engine.rootContext()->setContextProperty(QStringLiteral("tempsFanSpeeds"), &tempsFanSpeeds);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if(engine.rootObjects().isEmpty())
        return -1;

    if(!client.start(&cpu, &gpu))
    {
        qDebug().noquote() << client.lastError();

        return -1;
    }

//    QDBusServiceWatcher serviceWatcher("com.github.mlfc.server", QDBusConnection::sessionBus(), QDBusServiceWatcher::WatchForRegistration);


    return app.exec();
}

