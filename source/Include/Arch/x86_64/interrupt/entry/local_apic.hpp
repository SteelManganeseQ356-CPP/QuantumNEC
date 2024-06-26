#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PUBLIC using namespace QuantumNEC::Architecture;
    PUBLIC class LocalApicEntry
    {
    public:
        explicit( TRUE ) LocalApicEntry( VOID ) noexcept;
        virtual ~LocalApicEntry( VOID ) noexcept = default;

    private:
    };
}
