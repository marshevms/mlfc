#include "config.h"

#include <iostream>
#include <vector>
#include <fstream> //required for parse_file()
#include <filesystem>

#include <magic_enum.hpp>


namespace mlfc::config
{

Config::Config()
    : filePath_("mlfc.toml")
    , auto_(nullptr)
    , advanced_(nullptr)
    , currentMode_(nullptr)
{}

Config::Config(const std::string &path)
    : filePath_(path)
    , auto_(nullptr)
    , advanced_(nullptr)
{}

bool Config::read()
{
    auto_ = nullptr;
    advanced_ = nullptr;
    currentMode_ = nullptr;
    advancedTables_.clear();

    try
    {
        auto isExist = std::filesystem::exists(filePath_);

        if (!isExist)
        {
            config_ = toml::table{};
        }
        else
        {
            config_ = toml::parse_file(filePath_);
        }

        return validate();
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        setLastError(err.what());
        return false;
    }
    catch (const toml::parse_error &err)
    {
        return validate();
    }
    catch (...)
    {
        setLastError("unknown error happened");
        return false;
    }

    return false;
}

bool Config::save()
{

    try
    {
        auto isExist = std::filesystem::exists(filePath_);
        if (!isExist)
        {
            std::filesystem::path path{filePath_};
            auto dirPath = path.parent_path();

            if (!dirPath.empty()) std::filesystem::create_directories(path.parent_path());
        }

        std::fstream  file;
        file.open(filePath_, std::ios::out | std::ios::trunc);
        if (!file.is_open())
        {
            setLastError("can't open file: " + filePath_);
            return false;
        }

        file << config_;

        if (file.fail())
        {
            return false;
        }

        //file.close() called by the destructor
        return true;
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        setLastError(err.what());
        return false;
    }
    catch (...)
    {
        setLastError("unknown error happened");
        return false;
    }

    return true;
}

bool Config::setCurrentMode(const FanMode mode)
{
    switch (mode)
    {
    case FanMode::Auto:
        return setCurrentMode("auto");
    case FanMode::Advanced:
        return setCurrentMode("advanced");
    default:
        setLastError("mode is " + std::string{magic_enum::enum_name(mode)});
        return false;
    }
}

Config::FanMode Config::getCurrentMode() const
{
    if (!currentMode_)
        return FanMode::Unknown;

    if (!currentMode_->get().compare("auto"))
        return FanMode::Auto;

    if (!currentMode_->get().compare("advanced"))
        return FanMode::Advanced;

    return FanMode::Unknown;
}

bool Config::setCpuTemps(const std::vector<int> &temps, const FanMode mode, const std::string_view option)
{
    return setValueSwitch(temps, mode, cpuTempsSTR_, option);
}

bool Config::setCpuFanSpeeds(const std::vector<int> &speeds, FanMode mode, const std::string_view option)
{
    return setValueSwitch(speeds, mode, cpuFanSpeedsSTR_, option);
}

bool Config::setGpuTemps(const std::vector<int> &temps, FanMode mode, const std::string_view option)
{
    return setValueSwitch(temps, mode, gpuTempsSTR_, option);
}

bool Config::setGpuFanSpeeds(const std::vector<int> &speeds, const FanMode mode, std::string_view option)
{
    return setValueSwitch(speeds, mode, gpuFanSpeedsSTR_, option);
}

Config::Vector Config::getCpuTemps(const FanMode mode, const std::string_view option) const
{
    return getValueSwitch(mode, cpuTempsSTR_, option);
}

Config::Vector Config::getCpuFanSpeeds(const Config::FanMode mode, const std::string_view option) const
{
    return getValueSwitch(mode, cpuFanSpeedsSTR_, option);
}

Config::Vector Config::getGpuTemps(const Config::FanMode mode, const std::string_view option) const
{
    return getValueSwitch(mode, gpuTempsSTR_, option);
}

Config::Vector Config::getGpuFanSpeeds(const Config::FanMode mode, const std::string_view option) const
{
    return getValueSwitch(mode, gpuFanSpeedsSTR_, option);
}

std::string Config::getFilePath() const
{
    return filePath_;
}


std::string Config::lastError() const
{
    return lastError_;
}

void Config::setFilePath(const std::string &filePath)
{
    filePath_ = filePath;
}

bool Config::setCurrentMode(const std::string &mode)
{
    if (!currentMode_)
    {
        setLastError("node is nullptr");
        return false;
    }

    currentMode_->get().assign(mode);

    return true;
}

bool Config::setValueSwitch(const std::vector<int> &value, const FanMode mode, const std::string_view key, const std::string_view option)
{
    (void)option;

    switch (mode)
    {
    case FanMode::Auto:
        return setValue(auto_, key, value);
    case FanMode::Advanced:
        return setValue(advancedTables_.find(defaultStr_)->second, key, value);
    default:
        setLastError("mode can be Auto or Advanced");
        return false;
    }
}

bool Config::setValue(toml::table *tbl, const std::string_view key, const std::vector<int> &values)
{
    if (!tbl)
    {
        setLastError("tbl is nullptr");
        return false;
    }

    auto pair = pairs_.find(key);
    if (pair == pairs_.end())
    {
        setLastError("can't find key = " + std::string{key.data(), key.size()});
        return false;
    }

    if (pair->second != values.size())
    {
        setLastError("size of cpu must be equal + " + std::to_string(pair->second));
        return false;
    }

    tbl->as_table()->insert_or_assign(pair->first, toTomlArray(values));

    return true;
}

Config::Vector Config::getValueSwitch(const FanMode mode, const std::string_view key, const std::string_view option) const
{
    (void)option;

    switch (mode)
    {
    case FanMode::Auto:
        return getValue(auto_, key);
    case FanMode::Advanced:
        return getValue(advancedTables_.find(defaultStr_)->second, key);
    default:
        setLastError("mode can be Auto or Advanced");
        return {};
    }

    return {};
}

Config::Vector Config::getValue(const toml::table *tbl, const std::string_view key) const
{
    if (!tbl)
    {
        setLastError("tbl is nullptr");
        return {};
    }

    auto pair = pairs_.find(key);
    if (pair == pairs_.end())
    {
        setLastError("can't find key = " + std::string{key.data(), key.size()});
        return {};
    }

    auto node = tbl->find(key);
    if (node == tbl->end())
    {
        setLastError("can't find key = " + std::string{key.data(), key.size()});
        return {};
    }

    if (node->second.is_array())
        return toVector(*node->second.as_array());

    setLastError("node is not an array, it's is " + std::string{magic_enum::enum_name(node->second.type())});

    return {};
}

bool Config::validate()
{
    toml::node *node = nullptr;

    auto tbl = config_.as_table()->find(modeSTR_);
    auto currentMode = config_.as_table()->find(currentModeSTR_);

    //Mode
    if (tbl == config_.end())
    {
        if (node = createMode(&config_); !node) return false;
    }
    else if (!tbl->second.is_array_of_tables())
    {
        //User, Why do you do this?
        config_ = toml::table{};
        return validate();
    }
    else
    {
        node = tbl->second.as_array();
    }

    if (!validateMode(node)) return false;

    node = nullptr;

    //current_mode
    if (currentMode == config_.end())
    {
        node = config_.insert(currentModeSTR_, toml::value<std::string>{}).first->second.as_string();
    }
    else if (!currentMode->second.is_string())
    {
        //User, Why do you do this?
        config_ = toml::table{};
        return validate();
    }
    else
    {
        node = currentMode->second.as_string();
    }

    if (!validateCurrentMode(node)) return false;

    return true;
}

bool Config::validateMode(toml::node *node)
{
    if (!node)
    {
        setLastError("node is nullptr");
        return false;
    }

    if (!node->is_array())
    {
        setLastError("node is not an array, it's a " + std::string{magic_enum::enum_name(node->type())});
        return false;
    }

    bool isAutoExist, isAdvancedExist;
    isAutoExist = isAdvancedExist = false;

    for(auto &elem: *node->as_array())
    {
        if (elem.is_table())
        {
            if (auto result = elem.as_table()->find(autoSTR_); result != elem.as_table()->end())
            {
                if (auto tbl = result->second.as_table(); tbl)
                {
                    if (!validateAuto(tbl)) return false;
                    isAutoExist = true;
                    continue;
                }
            }

            if (auto result = elem.as_table()->find(advancedSTR_); result != elem.as_table()->end())
            {
                if(auto arr = result->second.as_array(); arr)
                {
                    if (!validateAdvanced(arr)) return false;
                    isAdvancedExist = true;
                    continue;
                }
            }
        }
        else
        {
            std::cout << "why this is '" << magic_enum::enum_name(elem.type()) << "' not a toml::table?" << std::endl;
        }
    }

    if (!isAutoExist)
        if (auto_ = createAuto(node); !auto_)
            return false;

    if (!isAdvancedExist)
        if (advanced_ = createAdvanced(node); !advanced_)
            return false;

    if (!validateAuto(auto_)) return false;
    if (!validateAdvanced(advanced_)) return false;

    return true;
}

bool Config::validateCurrentMode(toml::node *node)
{
    if (node == nullptr)
    {
        setLastError("node is nullptr");
        return false;
    }

    if (node->is_string())
    {
        currentMode_ = node->as_string();
        if (currentMode_->get().compare("auto") && currentMode_->get().compare("advanced"))
        {
            currentMode_->get().assign("auto");
        }
        return true;
    }

    setLastError("node is not a string, it's a " + std::string{magic_enum::enum_name(node->type())});

    return false;
}

bool Config::validateAuto(toml::node *node)
{
    if (node == nullptr)
    {
        setLastError("node is nullptr");
        return false;
    }

    if (node->is_table())
    {
        auto_ = node->as_table();
        return validateDefaultContent(auto_);
    }

    return false;
}

bool Config::validateAdvanced(toml::node *node)
{
    if (node == nullptr)
    {
        setLastError("node is nullptr");
        return false;
    }

    if (node->is_array())
    {
        advanced_ = node->as_array();

        for(auto &elem : *advanced_)
        {
            if (elem.is_table())
            {
                if (auto result = elem.as_table()->find(defaultStr_); result != elem.as_table()->end())
                {
                    if (auto tbl = result->second.as_table(); tbl)
                    {
                        auto res = validateDefaultContent(tbl);
                        if (res)
                        {
                            advancedTables_.emplace(defaultStr_, tbl);
                        }

                        return res;
                    }
                }
            }
        }

        auto tbl =  createTable(advanced_, defaultStr_);
        if (tbl != nullptr)
        {
            auto res = validateDefaultContent(tbl);
            if (res)
            {
                advancedTables_.emplace(defaultStr_, tbl);
            }

            return res;
        }
    }

    return false;
}

bool Config::validateDefaultContent(toml::node *node)
{
    if (!node)
    {
        setLastError("node is nullptr");
        return false;
    }

    if (node->is_table())
    {
        auto tbl = node->as_table();

        for (auto &key: pairs_)
        {
            toml::array *arr;
            auto pair = tbl->find(key.first);
            if (pair != tbl->end())
            {
                if (!pair->second.is_array())
                {
                    return false;
                }

                arr = pair->second.as_array();
            }
            else
            {
                if (arr = createArray(tbl, key.first); !arr) return false;
            }

            if(arr->empty())
                arr->reserve(key.second);

            //TO DO
            //Didn't find how to change the node exactly
            toml::array temp;
            temp.reserve(key.second);

            bool haveNotInt = false;
            for (auto iter = arr->begin(); iter != arr->end(); ++iter)
            {
                if (!iter->is_integer())
                {
                    haveNotInt = true;
                }
                temp.push_back(iter->value_or(0));
            }

            //User, I thought you were smart!
            //Sorry, but I just have to do it
            if (haveNotInt)
            {
                arr->clear();
                arr->reserve(temp.size());
                arr->insert(arr->begin(), temp.begin(), temp.end());
            }

        }

        return true;
    }

    setLastError("node is not a table, it's a " + std::string{magic_enum::enum_name(node->type())});

    return false;
}

toml::array *Config::createMode(toml::node *node)
{
    return createArray(node, modeSTR_);
}

toml::table *Config::createAuto(toml::node *node)
{
    return createTable(node, autoSTR_);
}

toml::array *Config::createAdvanced(toml::node *node)
{
    return createArray(node, advancedSTR_);
}

toml::array* Config::createArray(toml::node *node, const std::string_view key)
{
    if (!node)
    {
        setLastError("node is nullptr");
        return nullptr;
    }

    if (node->is_array())
    {
         auto res = node->as_array()->insert(node->as_array()->end(), toml::table{{{key, toml::array{}}}})->as_table()->find(key);

         //Can we have two or more equal keys?
         return res->second.is_array()? res->second.as_array(): nullptr;
    }

    if (node->is_table())
    {
        return node->as_table()->insert(key, toml::array{}).first->second.as_array();
    }

    setLastError("node is not a table or array, it's a " + std::string{magic_enum::enum_name(node->type())});

    return nullptr;
}

toml::table *Config::createTable(toml::node *node, const std::string_view key)
{
    if (!node)
    {
        setLastError("node is nullptr");
        return nullptr;
    }

    if (node->is_array())
    {
        auto res = node->as_array()->insert(node->as_array()->end(), toml::table{{{key, toml::table{}}}})->as_table()->find(key);

        //Can we have two or more equal keys?
        return res->second.is_table()? res->second.as_table(): nullptr;
    }

    if (node->is_table())
    {
        return node->as_table()->insert(key, toml::table{}).first->second.as_table();
    }

    setLastError("node is not a table or array, it's a " + std::string{magic_enum::enum_name(node->type())});

    return nullptr;
}

toml::array Config::toTomlArray(const std::vector<int> &vec) const
{
    toml::array res;
    res.reserve(vec.size());

    for(const auto &elem : vec)
    {
        res.push_back(elem);
    }

    return res;
}

std::vector<int> Config::toVector(const toml::array &arr) const
{
    if (!arr.is_homogeneous(toml::node_type::integer))
    {
        return {};
    }

    std::vector<int> res;
    res.reserve(arr.size());

    for(const auto &elem : arr)
    {
        res.push_back(elem.value<std::int64_t>().value());
    }

    return res;
}


void Config::setLastError(const std::string &&error) const
{
    lastError_ = std::move(error);
}

} // namespace mlfc::config
