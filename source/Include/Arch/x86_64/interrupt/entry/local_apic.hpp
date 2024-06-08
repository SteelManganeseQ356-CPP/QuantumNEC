#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#ifndef _INTERRUPT_HPP_
#error Do not include the header file. Please include interrupt.hpp
#endif
#ifndef _ENTRY_HPP_
#error Do not include the header file. Please include entry.hpp
#endif
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
