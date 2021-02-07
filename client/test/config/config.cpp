#include <gtest/gtest.h>

#include <filesystem>

#include "config.h"
#include "constants.h"

namespace
{

TEST(ConfigTest, full_test)
{
    mlfc::config::Config cfg;

    auto cpuTempsAuto = std::vector{1, 2, 3, 4, 5, 6};
    auto cpuFunSpeedsAuto = std::vector{7, 8, 9, 10, 11, 12, 13};

    auto gpuTempsAuto = std::vector{14, 15, 16, 17, 18, 19};
    auto gpuFunSpeedsAuto = std::vector{20, 21, 22, 23, 24, 25, 26};

    auto cpuTempsAdvanced = std::vector{1, 2, 3, 4, 5, 0};
    auto cpuFunSpeedsAdvanced = std::vector{7, 8, 9, 10, 11, 12, 0};

    auto gpuTempsAdvanced = std::vector{14, 15, 16, 17, 18, 0};
    auto gpuFunSpeedsAdvanced = std::vector{20, 21, 22, 23, 24, 25, 0};

    auto wrongSize = std::vector{0, 0, 0, 0, 0, 0, 0, 0};

    std::error_code ec;
    std::filesystem::remove(cfg.getFilePath(), ec);

    ASSERT_EQ(ec.value(), 0);

    ASSERT_NE(cfg.read(), false);

    //Current Mode
    EXPECT_EQ(cfg.getCurrentMode(), mlfc::EnumerationStorage::FanMode::Auto);
    EXPECT_EQ(cfg.setCurrentMode(mlfc::EnumerationStorage::FanMode::Advanced), true);
    EXPECT_EQ(cfg.getCurrentMode(), mlfc::EnumerationStorage::FanMode::Advanced);

    //Set Values
    //CPU
    //Auto
    EXPECT_EQ(cfg.setCpuTemps(cpuTempsAuto, mlfc::EnumerationStorage::FanMode::Auto), true);
    EXPECT_EQ(cfg.setCpuFanSpeeds(cpuFunSpeedsAuto, mlfc::EnumerationStorage::FanMode::Auto), true);

    //Advanced
    EXPECT_EQ(cfg.setCpuTemps(cpuTempsAdvanced, mlfc::EnumerationStorage::FanMode::Advanced), true);
    EXPECT_EQ(cfg.setCpuFanSpeeds(cpuFunSpeedsAdvanced, mlfc::EnumerationStorage::FanMode::Advanced), true);

    //GPU
    //Auto
    EXPECT_EQ(cfg.setGpuTemps(gpuTempsAuto, mlfc::EnumerationStorage::FanMode::Auto), true);
    EXPECT_EQ(cfg.setGpuFanSpeeds(gpuFunSpeedsAuto, mlfc::EnumerationStorage::FanMode::Auto), true);

    //Advanced
    EXPECT_EQ(cfg.setGpuTemps(gpuTempsAdvanced, mlfc::EnumerationStorage::FanMode::Advanced), true);
    EXPECT_EQ(cfg.setGpuFanSpeeds(gpuFunSpeedsAdvanced, mlfc::EnumerationStorage::FanMode::Advanced), true);

    //Wrong size
    //CPU
    EXPECT_EQ(cfg.setCpuTemps({cpuTempsAuto.begin(), cpuTempsAuto.end() - 1}, mlfc::EnumerationStorage::FanMode::Auto), false);
    EXPECT_EQ(cfg.setCpuFanSpeeds({cpuFunSpeedsAuto.begin(), cpuFunSpeedsAuto.end() - 1}, mlfc::EnumerationStorage::FanMode::Auto), false);

    EXPECT_EQ(cfg.setCpuTemps(wrongSize, mlfc::EnumerationStorage::FanMode::Auto), false);
    EXPECT_EQ(cfg.setCpuFanSpeeds(wrongSize, mlfc::EnumerationStorage::FanMode::Auto), false);

    //GPU
    EXPECT_EQ(cfg.setGpuTemps({gpuTempsAuto.begin(), gpuTempsAuto.end() - 1}, mlfc::EnumerationStorage::FanMode::Auto), false);
    EXPECT_EQ(cfg.setGpuFanSpeeds({gpuFunSpeedsAuto.begin(), gpuFunSpeedsAuto.end() - 1}, mlfc::EnumerationStorage::FanMode::Auto), false);

    EXPECT_EQ(cfg.setGpuTemps(wrongSize, mlfc::EnumerationStorage::FanMode::Auto), false);
    EXPECT_EQ(cfg.setGpuFanSpeeds(wrongSize, mlfc::EnumerationStorage::FanMode::Auto), false);

    //Get Values
    //CPU
    //Auto
    EXPECT_EQ(cfg.getCpuTemps(mlfc::EnumerationStorage::FanMode::Auto).value_or(std::vector<int>{}), cpuTempsAuto);
    EXPECT_EQ(cfg.getCpuFanSpeeds(mlfc::EnumerationStorage::FanMode::Auto).value_or(std::vector<int>{}), cpuFunSpeedsAuto);

    //Advanced
    EXPECT_EQ(cfg.getCpuTemps(mlfc::EnumerationStorage::FanMode::Advanced).value_or(std::vector<int>{}), cpuTempsAdvanced);
    EXPECT_EQ(cfg.getCpuFanSpeeds(mlfc::EnumerationStorage::FanMode::Advanced).value_or(std::vector<int>{}), cpuFunSpeedsAdvanced);

    //GPU
    //Auto
    EXPECT_EQ(cfg.getGpuTemps(mlfc::EnumerationStorage::FanMode::Auto).value_or(std::vector<int>{}), gpuTempsAuto);
    EXPECT_EQ(cfg.getGpuFanSpeeds(mlfc::EnumerationStorage::FanMode::Auto).value_or(std::vector<int>{}), gpuFunSpeedsAuto);

    //Advanced
    EXPECT_EQ(cfg.getGpuTemps(mlfc::EnumerationStorage::FanMode::Advanced).value_or(std::vector<int>{}), gpuTempsAdvanced);
    EXPECT_EQ(cfg.getGpuFanSpeeds(mlfc::EnumerationStorage::FanMode::Advanced).value_or(std::vector<int>{}), gpuFunSpeedsAdvanced);

    ASSERT_NE(cfg.save(), false);

    //Exactly from file
    ASSERT_NE(cfg.read(), false);

    //Current Mode
    EXPECT_EQ(cfg.getCurrentMode(), mlfc::EnumerationStorage::FanMode::Advanced);

    //Get Values
    //CPU
    //Auto
    EXPECT_EQ(cfg.getCpuTemps(mlfc::EnumerationStorage::FanMode::Auto).value_or(std::vector<int>{}), cpuTempsAuto);
    EXPECT_EQ(cfg.getCpuFanSpeeds(mlfc::EnumerationStorage::FanMode::Auto).value_or(std::vector<int>{}), cpuFunSpeedsAuto);

    //Advanced
    EXPECT_EQ(cfg.getCpuTemps(mlfc::EnumerationStorage::FanMode::Advanced).value_or(std::vector<int>{}), cpuTempsAdvanced);
    EXPECT_EQ(cfg.getCpuFanSpeeds(mlfc::EnumerationStorage::FanMode::Advanced).value_or(std::vector<int>{}), cpuFunSpeedsAdvanced);

    //GPU
    //Auto
    EXPECT_EQ(cfg.getGpuTemps(mlfc::EnumerationStorage::FanMode::Auto).value_or(std::vector<int>{}), gpuTempsAuto);
    EXPECT_EQ(cfg.getGpuFanSpeeds(mlfc::EnumerationStorage::FanMode::Auto).value_or(std::vector<int>{}), gpuFunSpeedsAuto);

    //Advanced
    EXPECT_EQ(cfg.getGpuTemps(mlfc::EnumerationStorage::FanMode::Advanced).value_or(std::vector<int>{}), gpuTempsAdvanced);
    EXPECT_EQ(cfg.getGpuFanSpeeds(mlfc::EnumerationStorage::FanMode::Advanced).value_or(std::vector<int>{}), gpuFunSpeedsAdvanced);

    ASSERT_NE(cfg.save(), false);
}

}
