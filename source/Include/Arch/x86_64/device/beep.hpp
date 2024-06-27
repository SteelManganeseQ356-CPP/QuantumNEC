#pragma once
#include <Lib/Types/Uefi.hpp>

PUBLIC namespace QuantumNEC::Architecture {
    PUBLIC constexpr CONST auto SPEAKER_REG { 0x61 };
    PUBLIC constexpr CONST auto BEEP_HZ { 440 };     // 声音为440HZ
    // PUBLIC constexpr CONST auto BEEP_COUNTER { Architecture::Device::OSCILLATOR / BEEP_HZ };
    PUBLIC class Beep     // 蜂鸣器管理
    {
    public:
        explicit Beep( VOID ) noexcept;
        virtual ~Beep( VOID ) noexcept;

    public:
        STATIC auto enable_beep( VOID ) -> VOID;
        STATIC auto disable_beep( VOID ) -> VOID;

    private:
        inline STATIC Lib::Types::uint64_t jiffies { };
        inline STATIC Lib::Types::uint64_t beeping { };
    };
}