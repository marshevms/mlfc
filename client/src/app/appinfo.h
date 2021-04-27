#ifndef APPINFO_H
#define APPINFO_H

#include <QObject>

#include <magic_enum.hpp>
#include <toml++/toml_version.h>

namespace mlfc {

struct Library{
    constexpr Library(const std::string_view name, const std::string_view link,
                      uint8_t major, uint8_t minor, uint8_t patch)
        : name(name),
          link(link),
          major(major),
          minor(minor),
          patch(patch){}

    ~Library() = default;

    std::string_view name;
    std::string_view link;

    uint8_t major;
    uint8_t minor;
    uint8_t patch;
};


class AppInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString commit READ commit NOTIFY commitChanged)
    Q_PROPERTY(QString usedLibrary READ usedLibrary NOTIFY usedLibraryChanged)

public:
    explicit AppInfo(QObject *parent = nullptr);

    QString version() const;
    QString commit() const;
    QString usedLibrary() const;

signals:
    void versionChanged();
    void commitChanged();
    void usedLibraryChanged();

private:
    static constexpr std::array<Library, 3> usedLibrary_{
        Library{{"Qt"}, {"https://github.com/qt"}, QT_VERSION_MAJOR, QT_VERSION_MINOR, QT_VERSION_PATCH},
        Library{{"Toml++"}, {"https://github.com/marzer/tomlplusplus"}, TOML_LIB_MAJOR, TOML_LIB_MINOR, TOML_LIB_PATCH},
        Library{{"Magic Enum C++"}, {"https://github.com/Neargye/magic_enum"}, MAGIC_ENUM_VERSION_MAJOR, MAGIC_ENUM_VERSION_MINOR, MAGIC_ENUM_VERSION_PATCH}
    };

};


} // namespace mlfc
#endif // APPINFO_H
