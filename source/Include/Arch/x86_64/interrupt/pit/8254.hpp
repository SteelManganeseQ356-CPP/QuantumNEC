#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#ifndef _INTERRUPT_HPP_
#error Do not include the header file. Please include register.hpp
#endif
#ifndef _PIT_HPP_
#error Do not include the header file. Please include entry.hpp
#endif
#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC constexpr CONST auto IRQ0_FREQUENCY { 100 };
PUBLIC constexpr CONST auto INPUT_FREQUENCY { 1193180 };
PUBLIC constexpr CONST auto COUNTER0_VALUE { INPUT_FREQUENCY / IRQ0_FREQUENCY };
PUBLIC constexpr CONST auto CONTRER0_PORT { 0x40 };
PUBLIC constexpr CONST auto COUNTER0_NO { 0 };
PUBLIC constexpr CONST auto COUNTER_MODE { 2 };
PUBLIC constexpr CONST auto READ_WRITE_LATCH { 3 };
PUBLIC constexpr CONST auto PIT_CONTROL_PORT { 0x43 };
PUBLIC namespace QuantumNEC::Architecture::Interrupt::PIT {
    PUBLIC class P8254Management
    {
    public:
        /**
         * @brief 初始化 8254 pit
         */
        explicit( true ) P8254Management( VOID ) noexcept;

        virtual ~P8254Management( VOID ) noexcept;
    };
}