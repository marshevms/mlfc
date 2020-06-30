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

core::FanMode ServerAdapter::FanMode()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->FanMode();
}

core::CoolerBoost ServerAdapter::CoolerBoost()
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->CoolerBoost();
}

bool ServerAdapter::SetCoolerBoost(core::CoolerBoost cooler_boost)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->SetCoolerBoost(cooler_boost);
}


bool ServerAdapter::SetUpdateInterval(int interval)
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->SetUpdateInterval(interval);
}

int ServerAdapter::UpdateInterval() const
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->UpdateInterval();
}

QString ServerAdapter::last_error() const
{
    return reinterpret_cast<mlfc::Server*>(this->parent())->last_error();
}

} // namespace mlfc
