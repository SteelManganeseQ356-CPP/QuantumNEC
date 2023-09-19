#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#ifndef _INTERRUPT_HPP_
#define _INTERRUPT_HPP_
#endif
#include <Lib/Types/Uefi.hpp>
#include <Arch/x86_64/interrupt/apic.hpp>
#include <Arch/x86_64/interrupt/pic_8258A.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt {
    PUBLIC class InterruptManagement :
#ifndef APIC
        Pic8259aManagement
#else
        ApicManagement
#endif
    {
    public:
        explicit( true ) InterruptManagement( VOID ) noexcept( true );

    public:
        virtual ~InterruptManagement( VOID ) noexcept( true );

    private:
    };
}