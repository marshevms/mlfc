#ifndef CPU_H
#define CPU_H

#include <QObject>

namespace mlfc
{

class CPU : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int temp READ temp NOTIFY tempChanged)
    Q_PROPERTY(int fanRPM READ fanRpm NOTIFY fanRpmChanged)
    Q_PROPERTY(QVector<int> temps READ temps NOTIFY tempsChanged)
    Q_PROPERTY(QVector<int> fanSpeeds READ fanSpeeds NOTIFY fanSpeedsChanged)
public:
    explicit CPU(QObject *parent = nullptr);

    int setTemp(int temp);
    int temp() const;

    int setFanRpm(int fan_rpm);
    int fanRpm() const;

    int setTemps(const QVector<int> &temps);
    QVector<int> temps();

    int setFanSpeeds(const QVector<int> &fanSpeeds);
    QVector<int> fanSpeeds();

signals:
    void tempChanged();
    void fanRpmChanged();
    void tempsChanged();
    void fanSpeedsChanged();

private:
    int temp_;
    int fanRpm_;

    QVector<int> temps_;
    QVector<int> fanSpeeds_;
};

} // namespace mlfc
#endif // CPU_H
