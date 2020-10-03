#ifndef GPU_H
#define GPU_H

#include <QObject>

namespace mlfc
{

class GPU : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int temp READ temp NOTIFY tempChanged)
    Q_PROPERTY(int fanRPM READ fanRpm NOTIFY fanRpmChanged)
public:
    explicit GPU(QObject *parent = nullptr);

    int setTemp(int temp);
    int temp() const;

    int setFanRpm(int fanRpm);
    int fanRpm() const;

signals:
    void tempChanged();
    void fanRpmChanged();

private:
    int temp_;
    int fanRpm_;

};

} // namespace mlfc
#endif // GPU_H
