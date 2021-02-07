#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include <QVector>

namespace mlfc::model
{

struct TempsFanSpeeds
{
    TempsFanSpeeds() = default;
    TempsFanSpeeds(const TempsFanSpeeds&) = default;
    TempsFanSpeeds(const std::vector<int> &temps, const std::vector<int> &fanSpeeds);
    TempsFanSpeeds(std::vector<int> &&temps, std::vector<int> &&fanSpeeds);

    TempsFanSpeeds &operator=(const TempsFanSpeeds &value);
    TempsFanSpeeds &operator=(TempsFanSpeeds &&value);

    [[nodiscard]] bool isEqual(const TempsFanSpeeds &value) const;

    std::vector<int> temps;
    std::vector<int> fanSpeeds;

private:
    void reserve();
    void resize();
};

struct CpuGpu
{
    CpuGpu() = default;
    CpuGpu(const CpuGpu&) = default;
    CpuGpu(const TempsFanSpeeds &cpu, const TempsFanSpeeds &gpu);
    CpuGpu(TempsFanSpeeds &&cpu, TempsFanSpeeds &&gpu);

    [[nodiscard]] bool isEqual(const CpuGpu &value) const;

    TempsFanSpeeds cpu;
    TempsFanSpeeds gpu;
};

bool isEmpty(const CpuGpu &pair);

inline QVector<int> toQVector(const std::vector<int> &vec)
{
    return {vec.begin(), vec.end()};
}

} // namespace mlfc::model

#endif // MODEL_H
