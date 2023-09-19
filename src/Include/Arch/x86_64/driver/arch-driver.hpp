#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#ifndef _ARCH_DRIVER_HPP_
#define _ARCH_DRIVER_HPP_
#endif
#include <Lib/Types/Uefi.hpp>
#include <Arch/x86_64/driver/timer.hpp>
PUBLIC namespace QuantumNEC::Architecture::Driver {
    PUBLIC class DriverManagement : TimerDriverManagement
    {
    public:
        explicit( true ) DriverManagement( VOID ) noexcept( true );

    public:
        virtual ~DriverManagement( VOID ) noexcept( true );

    private:
    };
}