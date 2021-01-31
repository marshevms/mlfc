#ifndef QMLTEMPSFANSPEED_H
#define QMLTEMPSFANSPEED_H

#include <QObject>

#include "model.h"

namespace mlfc::model
{

    class qmlTempsFanSpeeds : public QObject
    {
        Q_OBJECT
    public:
        qmlTempsFanSpeeds();
        qmlTempsFanSpeeds(const TempsFanSpeeds &tempsFanSpeeds);
        qmlTempsFanSpeeds(TempsFanSpeeds &&tempsFanSpeeds);
        ~qmlTempsFanSpeeds() = default;

        Q_INVOKABLE void setTempsFanSpeeds(const QVector<QPoint> &empsFanSpeeds);

        TempsFanSpeeds tempsFanSpeeds;

    private:
        void clear();
        void reserve();
    };

} // namespace mlfc::model

#endif // QMLTEMPSFANSPEED_H
