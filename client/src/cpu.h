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
public:
    explicit CPU(QObject *parent = nullptr);

    int setTemp(int temp);
    int temp() const;

    int setFanRpm(int fan_rpm);
    int fanRpm() const;

signals:
    void tempChanged();
    void fanRpmChanged();

private:
    int temp_;
    int fanRpm_;

};

} // namespace mlfc
#endif // CPU_H
