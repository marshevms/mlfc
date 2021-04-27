#include "appinfo.h"

#include <toml++/toml.h>

#include "client_info.h"

namespace mlfc {

AppInfo::AppInfo(QObject *parent) : QObject(parent)
{}

QString AppInfo::version() const
{
    return PROJECT_VER;
}

QString AppInfo::commit() const
{
    return GIT_COMMIT_HASH;
}

QString AppInfo::usedLibrary() const
{
    QString li("<li>%1</li>");
    QString a("<a href=\"%1\">%2</a> %3.%4.%5");

    std::vector<QString> libs(usedLibrary_.size());

    for(const auto &lib : usedLibrary_)
    {
        libs.push_back(li
                       .arg(
                           a
                           .arg(QString::fromUtf8(lib.link.data(), lib.link.size()),
                                QString::fromUtf8(lib.name.data(), lib.name.size()))
                           .arg(lib.major).arg(lib.minor).arg(lib.patch)
                           )
                       );
    }

    uint32_t length = std::string("<ul></ul>").size();

    for(const auto &lib : libs){
        length += lib.size();
    }

    QString res;
    res.reserve(length);
    res.append("<ul>");

    for(const auto &lib : libs){
        res.append(lib);
    }

    res.append("</ul>");

    return res;
}

} // namespace mlfc
