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
PUBLIC namespace QuantumNEC::Architecture::Interrupt::PIT {
    PUBLIC class HpetManagement
    {
    public:
        struct HpetTimer
        {
            Lib::Types::uint64_t configurationAndCapability;
            Lib::Types::uint64_t comparatorValue;
            Lib::Types::uint64_t fsbInterruptRoute;
            Lib::Types::uint64_t reserved;
        } _packed;
        struct HpetInfo
        {
            Lib::Types::uint64_t generalCapabilities;
            Lib::Types::uint64_t reserved0;
            Lib::Types::uint64_t generalConfiguration;
            Lib::Types::uint64_t reserved1;
            Lib::Types::uint64_t generalIntrruptStatus;
            Lib::Types::uint8_t reserved3[ 200 ];
            Lib::Types::uint64_t mainCounterValue;
            Lib::Types::uint64_t reserved4;
            HpetTimer timers[ 0 ];
        } _packed;

    public:
        /**
         * @brief 初始化 HEPT
         */
        explicit( true ) HpetManagement( VOID ) noexcept;

        virtual ~HpetManagement( VOID ) noexcept;
    };
}