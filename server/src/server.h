#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include <QObject>


class QTimer;

namespace mlfc
{

namespace core
{

class Reader;

}

}

namespace mlfc
{

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();


    void set_last_error(const QString &error);

signals:
    void RealtimeCPUTemp(int temp);
    void RealtimeCPUFanRPM(int rpm);

    void RealtimeGPUTemp(int temp);
    void RealtimeGPUFanRPM(int rpm);

    void AnErrorOccured(QString error);

public slots:
    bool Start();

    bool SetUpdateInterval(int interval);
    int UpdateInterval() const;

    QString last_error() const;

private:
    static constexpr int kMinTimeoutSecs = 500;
    static constexpr int kMaxTimeoutSecs = 3000;
    static constexpr int kTimeoutSecs = 1000;

    QTimer *timer_;
    std::unique_ptr<core::Reader> reader_;

    QString last_error_;

    void Update();

};

} // namespace mlfc

#endif // SERVER_H
