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
        ONN,
        OFF,
        Unknown
    };
    Q_ENUM(CoolerBoost)
};

} // namespace mlfc
#endif // ENUMERATIONSTORAGE_H
