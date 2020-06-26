#ifndef GPU_H
#define GPU_H

#include <QObject>

namespace mlfc
{

class GPU : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int temp READ temp NOTIFY temp_changed)
    Q_PROPERTY(int fanRPM READ fan_rpm NOTIFY fan_rpm_changed)
public:
    explicit GPU(QObject *parent = nullptr);

    int set_temp(int temp);
    int temp() const;

    int set_fan_rpm(int fan_rpm);
    int fan_rpm() const;

signals:
    void temp_changed();
    void fan_rpm_changed();

private:
    int temp_;
    int fan_rpm_;

};

} // namespace mlfc
#endif // GPU_H
