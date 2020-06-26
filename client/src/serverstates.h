#ifndef SERVERSTATES_H
#define SERVERSTATES_H

#include <qobjectdefs.h>

namespace mlfc
{
Q_NAMESPACE
enum class ServerStates
{
    None,
    Starting,
    Working,
    Stopping,
    Stopped
};
Q_ENUM_NS(ServerStates)
} // namespace mlfc

#endif // SERVERSTATES_H
