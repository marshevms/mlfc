#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include <QObject>


class QTimer;

namespace mlfc::core
{
class Manager;
enum class FanMode;
enum class CoolerBoost;
}

namespace mlfc
{

class Server : public QObject
{
    Q_OBJECT
public:

    enum class ServerStatus
    {
        kON,
        kOFF
    };

    explicit Server(QObject *parent = nullptr);
    ~Server();


    void setLastError(const QString &error);

signals:
    void realtimeCPUTemp(int temp);
    void realtimeCPUFanRPM(int rpm);

    void realtimeGPUTemp(int temp);
    void realtimeGPUFanRPM(int rpm);

    void anErrorOccured(QString error);

public slots:
    bool start();

    enum core::FanMode fanMode();
    enum core::CoolerBoost coolerBoost();

    bool setCoolerBoost(core::CoolerBoost cooler_boost);
    bool setFanMode(core::FanMode fan_mode);

    QVector<int> cpuTemps();
    QVector<int> cpuFanSpeeds();

    QVector<int> gpuTemps();
    QVector<int> gpuFanSpeeds();

    bool setCPUTemps(QVector<int> &cpuTemps);
    bool setCPUFanSpeeds(QVector<int> &cpuFanSpeeds);

    bool setGPUTemps(QVector<int> &gpuTemps);
    bool setGPUFanSpeeds(QVector<int> &gpuFanSpeeds);

    QString ecVersion();

    bool setUpdateInterval(int interval);
    int updateInterval() const;

    QString lastError() const;

private:
    static constexpr int kMinTimeoutSecs = 500;
    static constexpr int kMaxTimeoutSecs = 3000;
    static constexpr int kTimeoutSecs = 1000;

    ServerStatus server_status_;

    QTimer *timer_;
    std::unique_ptr<core::Manager> manager_;

    QString last_error_;

    void update();

};

} // namespace mlfc

#endif // SERVER_H
