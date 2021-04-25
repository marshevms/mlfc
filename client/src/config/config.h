#ifndef CONFIG_H
#define CONFIG_H

#include <string_view>

#include <toml++/toml.h>

#include "enumerationstorage.h"
#include "constants.h"

namespace mlfc::config
{

class Config
{
    using FanMode = EnumerationStorage::FanMode;
    using IconTheme = EnumerationStorage::IconTheme;
    using Vector = std::optional<std::vector<int>>;
public:
    Config();
    Config(const std::string &path);

    [[nodiscard]] bool read();
    [[nodiscard]] bool save();

    [[nodiscard]] bool setCurrentMode(const FanMode mode);
    FanMode getCurrentMode() const;

    [[nodiscard]] bool setIconTheme(const IconTheme theme);
    IconTheme getIconTheme() const;

    //Set cpu values
    [[nodiscard]] bool setCpuTemps(const std::vector<int> &temps, const FanMode mode, const std::string_view option = "");
    [[nodiscard]] bool setCpuFanSpeeds(const std::vector<int> &speeds, const FanMode mode, const std::string_view option = "");

    //Set gpu values
    [[nodiscard]] bool setGpuTemps(const std::vector<int> &temps, const FanMode mode, const std::string_view option = "");
    [[nodiscard]] bool setGpuFanSpeeds(const std::vector<int> &speeds, const FanMode mode, const std::string_view option = "");

    //Get cpu values
    Vector getCpuTemps(const FanMode mode, const std::string_view option = "") const;
    Vector getCpuFanSpeeds(const FanMode mode, const std::string_view option = "") const;

    //Get gpu values
    Vector getGpuTemps(const FanMode mode, const std::string_view option = "") const;
    Vector getGpuFanSpeeds(const FanMode mode, const std::string_view option = "") const;

    void setFilePath(const std::string &filePath);
    std::string getFilePath() const;

    std::string lastError() const;

private:

    static constexpr std::string_view modeSTR_         {"mode"};
    static constexpr std::string_view iconThemeStr_    {"icon_theme"};
    static constexpr std::string_view currentModeSTR_  {"current_mode"};
    static constexpr std::string_view autoSTR_         {"auto"};
    static constexpr std::string_view advancedSTR_     {"advanced"};

    static constexpr std::string_view defaultStr_      {"default"};

    static constexpr std::string_view cpuTempsSTR_     {"cpu_temps"};
    static constexpr std::string_view cpuFanSpeedsSTR_ {"cpu_fan_speeds"};

    static constexpr std::string_view gpuTempsSTR_     {"gpu_temps"};
    static constexpr std::string_view gpuFanSpeedsSTR_ {"gpu_fan_speeds"};

    static const inline std::map<std::string_view, uint> pairs_ =
    {
        {cpuTempsSTR_, kTempsNumber},
        {cpuFanSpeedsSTR_, kFanSpeedsNumber},
        {gpuTempsSTR_, kTempsNumber},
        {gpuFanSpeedsSTR_, kFanSpeedsNumber}
    };

    std::string filePath_;

    toml::table config_;
    toml::table *auto_;
    toml::array *advanced_;
    toml::value<std::string> *currentMode_;
    toml::value<std::string> *currentIconTheme_;

    std::map<std::string_view, toml::table*> advancedTables_;

    mutable std::string lastError_;

    //Set
    [[nodiscard]] bool setCurrentIconTheme(const std::string &icon_theme);
    [[nodiscard]] bool setCurrentMode(const std::string &mode);
    [[nodiscard]] bool setValueSwitch(const std::vector<int> &value, const FanMode mode, const std::string_view key, const std::string_view option);
    [[nodiscard]] bool setValue(toml::table *tbl, const std::string_view key, const std::vector<int> &values);

    //Get
    Vector getValueSwitch(const FanMode mode, const std::string_view key, const std::string_view option) const;
    Vector getValue(const toml::table *tbl, const std::string_view key) const;

    //Validate and create default node for config
    [[nodiscard]] bool validate();

    [[nodiscard]] bool validateIconTheme(toml::node *node);
    [[nodiscard]] bool validateCurrentMode(toml::node *node);
    [[nodiscard]] bool validateMode(toml::node *node);
    [[nodiscard]] bool validateAuto(toml::node *node);
    [[nodiscard]] bool validateAdvanced(toml::node *node);

    [[nodiscard]] bool validateDefaultContent(toml::node *node);

    [[nodiscard]] toml::array* createMode(toml::node *node);
    [[nodiscard]] toml::table* createAuto(toml::node *node);
    [[nodiscard]] toml::array* createAdvanced(toml::node *node);

    void setLastError(const std::string &&error) const;

    //Helpers
    [[nodiscard]] toml::array* createArray(toml::node *node, const std::string_view key);
    [[nodiscard]] toml::table* createTable(toml::node *node, const std::string_view key);

    toml::array toTomlArray(const std::vector<int> &vec) const;

    std::vector<int> toVector(const toml::array &arr) const;
};

} // namespace mlfc::config

#endif // CONFIG_H
