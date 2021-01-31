#include "qmltempsfanspeeds.h"

#include <QPoint>

#include <constants.h>

namespace mlfc::model
{

qmlTempsFanSpeeds::qmlTempsFanSpeeds()
{}

qmlTempsFanSpeeds::qmlTempsFanSpeeds(const TempsFanSpeeds &tempsFanSpeeds)
{
    reserve();

    std::copy(tempsFanSpeeds.temps.begin(), tempsFanSpeeds.temps.end(), std::back_insert_iterator(this->tempsFanSpeeds.temps));
    std::copy(tempsFanSpeeds.fanSpeeds.begin(), tempsFanSpeeds.fanSpeeds.end(), std::back_insert_iterator(this->tempsFanSpeeds.fanSpeeds));
}

qmlTempsFanSpeeds::qmlTempsFanSpeeds(TempsFanSpeeds &&tempsFanSpeeds)
{
    this->tempsFanSpeeds = std::move(tempsFanSpeeds);
}

void qmlTempsFanSpeeds::clear()
{
    this->tempsFanSpeeds.fanSpeeds.clear();
    this->tempsFanSpeeds.temps.clear();
}

void qmlTempsFanSpeeds::reserve()
{
    this->tempsFanSpeeds.temps.reserve(kTempsNumber);
    this->tempsFanSpeeds.fanSpeeds.reserve(kFanSpeedsNumber);
}

void qmlTempsFanSpeeds::setTempsFanSpeeds(const QVector<QPoint> &tempsFanSpeeds)
{
    clear();
    reserve();

    for (int i = 0; i < tempsFanSpeeds.size() && i < 1; i++)
    {
        this->tempsFanSpeeds.fanSpeeds.emplace_back(tempsFanSpeeds[i].y());
    }

    for (int i = 1; i < tempsFanSpeeds.size(); i++)
    {
        this->tempsFanSpeeds.temps.emplace_back(tempsFanSpeeds[i].x());
        this->tempsFanSpeeds.fanSpeeds.emplace_back(tempsFanSpeeds[i].y());
    }
}

}
