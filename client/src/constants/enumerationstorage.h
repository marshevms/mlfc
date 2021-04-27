#ifndef ENUMERATIONSTORAGE_H
#define ENUMERATIONSTORAGE_H

#include <QObject>

namespace mlfc
{

class EnumerationStorage
{
    Q_GADGET
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
public:

    EnumerationStorage() = delete;

    enum class ServerStates
    {
        Starting,
        Working,
        Stopping,
        Stopped,
        Unknown
    };
    Q_ENUM(ServerStates)

    enum class FanMode
    {
        Auto,
        Basic,
        Advanced,
        Unknown
    };
    Q_ENUM(FanMode)

    enum class CoolerBoost
    {
        ON,
        OFF,
        Unknown
    };
    Q_ENUM(CoolerBoost)

    enum class ChartValues
    {
        CPU,
        GPU
    };
    Q_ENUM(ChartValues)

    enum class IconTheme
    {
        Light,
        Dark,
        Unknown
    };
    Q_ENUM(IconTheme)
};

} // namespace mlfc
#endif // ENUMERATIONSTORAGE_H
