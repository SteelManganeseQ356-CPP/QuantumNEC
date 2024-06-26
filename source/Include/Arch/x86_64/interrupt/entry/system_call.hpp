#pragma once
#include <Lib/Types/Uefi.hpp>

PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PUBLIC using namespace QuantumNEC::Architecture;
    PUBLIC class SystemcallEntry
    {
    public:
        explicit( TRUE ) SystemcallEntry( VOID ) noexcept;
        virtual ~SystemcallEntry( VOID ) noexcept = default;

    private:
    };
}