#pragma once
#include <Lib/Types/Uefi.hpp>
namespace QuantumNEC::Architecture::Device {
constexpr CONST auto HZ { 100 };
constexpr CONST auto OSCILLATOR { 1193182 };
constexpr CONST auto CLOCK_COUNTER { OSCILLATOR / HZ };
constexpr CONST auto JIFFY { 1000 / HZ };
}     // namespace QuantumNEC::Architecture::Device
