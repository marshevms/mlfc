#ifndef FANMODEMODEL_H
#define FANMODEMODEL_H

#include <QAbstractListModel>

namespace mlfc
{

class FanModeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FanModeModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:

    enum {
        TextRole = Qt::UserRole,
        ValueRole
    };

    const int rowCount_;
    QVector<QString> valueKey_;
    QVector<QVariant> value_;


    constexpr static int ignoreUnknowValue_ = 1;
};

} // mlfc

#endif // FANMODEMODEL_H
