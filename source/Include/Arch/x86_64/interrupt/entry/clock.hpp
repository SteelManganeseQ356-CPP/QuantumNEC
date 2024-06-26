#pragma once
#include <Lib/Types/Uefi.hpp>

PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PUBLIC using namespace QuantumNEC::Architecture;
    PUBLIC class ClockEntry
    {
    public:
        explicit( TRUE ) ClockEntry( VOID ) noexcept;
        virtual ~ClockEntry( VOID ) noexcept = default;

    private:
    };
}
