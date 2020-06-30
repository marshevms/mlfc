#include "serverinterface.h"

namespace mlfc
{

ServerInterface::ServerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    :QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{}

}
