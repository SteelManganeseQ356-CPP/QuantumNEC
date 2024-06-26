#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PUBLIC using namespace QuantumNEC::Architecture;
    PUBLIC class RTCEntry
    {
    public:
        explicit( TRUE ) RTCEntry( VOID ) noexcept;
        virtual ~RTCEntry( VOID ) noexcept = default;

    private:
    };
}
