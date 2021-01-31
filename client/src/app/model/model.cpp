#include "model.h"

#include <constants.h>

namespace mlfc::model
{

TempsFanSpeeds::TempsFanSpeeds(const std::vector<int> &temps, const std::vector<int> &fanSpeeds)
{
    reserve();

    std::copy(temps.begin(), temps.end(), std::back_insert_iterator(this->temps));
    std::copy(fanSpeeds.begin(), fanSpeeds.end(), std::back_insert_iterator(this->fanSpeeds));
}

TempsFanSpeeds::TempsFanSpeeds(std::vector<int> &&temps, std::vector<int> &&fanSpeeds)
{
    this->temps = std::move(temps);
    this->fanSpeeds = std::move(fanSpeeds);

    resize();
}

TempsFanSpeeds &TempsFanSpeeds::operator=(const TempsFanSpeeds &value)
{
    reserve();

    std::copy(value.temps.begin(), value.temps.end(), std::back_insert_iterator(this->temps));
    std::copy(value.fanSpeeds.begin(), value.fanSpeeds.end(), std::back_insert_iterator(this->fanSpeeds));

    return *this;
}

TempsFanSpeeds &TempsFanSpeeds::operator=(TempsFanSpeeds &&value)
{
    this->temps = std::move(value.temps);
    this->fanSpeeds = std::move(value.fanSpeeds);

    resize();

    return *this;
}

bool TempsFanSpeeds::isEqual(const TempsFanSpeeds &value) const
{
    bool ok = true;

    ok &= std::equal(temps.begin(), temps.end(), value.temps.begin());
    ok &= std::equal(fanSpeeds.begin(), fanSpeeds.end(), value.fanSpeeds.begin());

    return ok;
}

void TempsFanSpeeds::reserve()
{
    temps.reserve(kTempsNumber);
    fanSpeeds.reserve(kFanSpeedsNumber);
}

void TempsFanSpeeds::resize()
{
    this->temps.resize(kTempsNumber);
    this->fanSpeeds.resize(kFanSpeedsNumber);
}

bool isEmpty(const mlfc::model::CpuGpu &pair)
{
    return pair.cpu.temps.empty() || pair.cpu.fanSpeeds.empty()
            || pair.gpu.temps.empty() || pair.gpu.fanSpeeds.empty();
}

CpuGpu::CpuGpu(const TempsFanSpeeds &cpu, const TempsFanSpeeds &gpu)
{
    this->cpu = cpu;
    this->gpu = gpu;
}

CpuGpu::CpuGpu(TempsFanSpeeds &&cpu, TempsFanSpeeds &&gpu)
{
    this->cpu = std::move(cpu);
    this->gpu = std::move(gpu);
}

bool CpuGpu::isEqual(const CpuGpu &value) const
{
    return this->cpu.isEqual(value.cpu) && this->gpu.isEqual(value.gpu);
};


} //namespace mlfs::model

