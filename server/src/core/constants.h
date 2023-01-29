#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>
#include <string>

namespace mlfc::core {

static const std::string driver_path = "/sys/firmware/mlekm";

static const std::string realtime_cpu_temp_path = driver_path + "/cpu/realtime_temp";
static const std::string realtime_cpu_fan_rpm_path = driver_path + "/cpu/realtime_fan_rpm";
static const std::string cpu_temps_path = driver_path + "/cpu/temps";
static const std::string cpu_fan_speeds_path = driver_path + "/cpu/fan_speeds";

static const std::string realtime_gpu_temp_path = driver_path + "/gpu/realtime_temp";
static const std::string realtime_gpu_fan_rpm_path = driver_path + "/gpu/realtime_fan_rpm";
static const std::string gpu_temps_path = driver_path + "/gpu/temps";
static const std::string gpu_fan_speeds_path = driver_path + "/gpu/fan_speeds";

static const std::string fan_mode_path = driver_path + "/fan_mode";
static const std::string cooler_boost_status_path = driver_path + "/cooler_boost_status";
static const std::string ec_version_path = driver_path + "/ec_version";

enum class CoolerBoost {
    ONN,
    OFF,
    Unknown
};

enum class FanMode {
    Auto,
    Basic,
    Advanced,
    Unknown
};

} // namespace mlfc::core

#endif // CONSTANTS_H
