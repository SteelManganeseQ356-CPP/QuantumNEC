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
PUBLIC namespace QuantumNEC::Architecture {
    PUBLIC class P8254
    {
    public:
        /**
         * @brief 初始化 8254 pit
         */
        explicit P8254( VOID ) noexcept;

        virtual ~P8254( VOID ) noexcept;
    };
}