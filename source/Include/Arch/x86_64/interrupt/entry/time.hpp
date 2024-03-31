#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#ifndef _INTERRUPT_HPP_
#error Do not include the header file. Please include register.hpp
#endif
#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
using namespace QuantumNEC::Architecture;

PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PUBLIC class TimeEntry
    {
    public:
        explicit( TRUE ) TimeEntry( VOID ) noexcept( TRUE );
        virtual ~TimeEntry( VOID )  noexcept( TRUE ) = default;

    private:
    };
}
