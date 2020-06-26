#include "serveradapter.h"

#include "server.h"

namespace mlfc
{

ServerAdapter::ServerAdapter(QObject *parent)
    :QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

bool ServerAdapter::Start()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->Start();
}

QString ServerAdapter::last_error()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->last_error();
}

} // namespace mlfc
