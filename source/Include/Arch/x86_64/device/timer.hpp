#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#ifndef _ARCH_DRIVER_HPP_
#error Do not include the header file. Please include device.hpp
#endif
#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::Device {
    PUBLIC constexpr CONST auto IRQ0_FREQUENCY { 100 };
    PUBLIC constexpr CONST auto INPUT_FREQUENCY { 1193180 };
    PUBLIC constexpr CONST auto COUNTER0_VALUE { INPUT_FREQUENCY / IRQ0_FREQUENCY };
    PUBLIC constexpr CONST auto CONTRER0_PORT { 0x40 };
    PUBLIC constexpr CONST auto COUNTER0_NO { 0 };
    PUBLIC constexpr CONST auto COUNTER_MODE { 2 };
    PUBLIC constexpr CONST auto READ_WRITE_LATCH { 3 };
    PUBLIC constexpr CONST auto PIT_CONTROL_PORT { 0x43 };
    PUBLIC class TimerDeviceManagement
    {
    public:
        /**
         * @brief 初始化 PIT 8253
         */
        explicit( true ) TimerDeviceManagement( VOID ) noexcept( true );

    public:
        virtual ~TimerDeviceManagement( VOID ) noexcept( true );

    private:
        /**
         * @brief 写入模式控制寄存器并赋予初始值
         * @param counter_no 操作的计数器
         * @param rwl 读写锁属性
         * @param counter_mode 计数器模式
         * @param counter_value 初始值
         */
        auto frequencySet(
            IN Lib::Types::uint8_t counter_port,
            IN Lib::Types::uint8_t counter_no,
            IN Lib::Types::uint8_t rwl,
            IN Lib::Types::uint8_t counter_mode,
            IN Lib::Types::uint16_t counter_value )
            -> VOID;

    private:
    };
}