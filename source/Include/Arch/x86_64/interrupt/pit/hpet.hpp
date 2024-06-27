#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    PUBLIC class Hpet
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
        explicit Hpet( VOID ) noexcept;

        virtual ~Hpet( VOID ) noexcept;
    };
}