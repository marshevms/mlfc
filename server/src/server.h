#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include <QObject>

class QTimer;

namespace mlfc::core {
class Manager;
enum class FanMode;
enum class CoolerBoost;
}

namespace mlfc {

class Server : public QObject {
    Q_OBJECT
public:
    enum class ServerStatus {
        kON,
        kOFF
    };

    explicit Server(QObject* parent = nullptr);
    ~Server();

    void setLastError(const QString& error);

signals:
    void anErrorOccured(QString error);

public slots:
    int realtimeCPUTemp();
    int realtimeCPUFanRPM();

    int realtimeGPUTemp();
    int realtimeGPUFanRPM();

    enum core::FanMode fanMode();
    enum core::CoolerBoost coolerBoost();

    bool setCoolerBoost(core::CoolerBoost cooler_boost);
    bool setFanMode(core::FanMode fan_mode);

    QVector<int> cpuTemps();
    QVector<int> cpuFanSpeeds();

    QVector<int> gpuTemps();
    QVector<int> gpuFanSpeeds();

    bool setCPUTemps(QVector<int>& cpuTemps);
    bool setCPUFanSpeeds(QVector<int>& cpuFanSpeeds);

    bool setGPUTemps(QVector<int>& gpuTemps);
    bool setGPUFanSpeeds(QVector<int>& gpuFanSpeeds);

    QString ecVersion();

    QString serverVersion();

    QString lastError() const;

private:
    ServerStatus server_status_;
    std::unique_ptr<core::Manager> manager_;

    QString last_error_;
};

} // namespace mlfc

#endif // SERVER_H
