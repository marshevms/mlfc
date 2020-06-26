#ifndef SERVERADAPTER_H
#define SERVERADAPTER_H

#include <QtDBus/QDBusAbstractAdaptor>

namespace mlfc
{

class ServerAdapter: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.mlfc.server")
public:
    ServerAdapter(QObject *parent);

signals:
    void RealtimeCPUTemp(int temp);
    void RealtimeCPUFanRPM(int rpm);

    void RealtimeGPUTemp(int temp);
    void RealtimeGPUFanRPM(int rpm);

public slots:
    bool Start();
    QString last_error();
};

} // namespace mlfc

#endif // SERVERADAPTER_H
