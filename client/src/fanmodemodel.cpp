#include "fanmodemodel.h"

#include <QDebug>
#include <QMetaEnum>

#include "enumerationstorage.h"

namespace mlfc
{

using FanMode = EnumerationStorage::FanMode;

FanModeModel::FanModeModel(QObject *parent)
    : QAbstractListModel(parent)
    , rowCount_(QMetaEnum::fromType<FanMode>().keyCount() - ignoreUnknowValue_)
{
    auto metaEnum = QMetaEnum::fromType<FanMode>();

    valueKey_.reserve(rowCount_);
    value_.reserve(rowCount_);
    for(int i = 0; i < rowCount_; ++i)
    {
        valueKey_.push_back(QString(metaEnum.valueToKey(i)));
        value_.push_back(QVariant::fromValue(static_cast<FanMode>(i)));
    }
}

QHash<int, QByteArray> FanModeModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[TextRole] = "textRole";
    names[ValueRole] = "valueRole";

    return names;
}

int FanModeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return rowCount_;
}

QVariant FanModeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    switch (role)
    {
    case TextRole:
        return valueKey_[index.row()];
    case ValueRole:
        return value_[index.row()];
    default:
        qDebug() << QString("ERROR: unknown role '%1'").arg(role);
    }

    return {};
}

} // mlfc
