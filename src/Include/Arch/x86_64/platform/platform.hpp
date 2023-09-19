#pragma once
#ifndef _PLATFORM_HPP_
#define _PLATFORM_HPP_
#endif
#include <Arch/x86_64/abi/builtins.hpp>
#include <Arch/x86_64/abi/cxxabi.hpp>
#include <Arch/x86_64/cpu/cpu.hpp>
#include <Arch/x86_64/cpu/xdt.hpp>
#include <Arch/x86_64/cpu/smp.hpp>
#include <Arch/x86_64/driver/arch-driver.hpp>
#include <Arch/x86_64/interrupt/interrupt.hpp>
#include <Arch/x86_64/platform/global.hpp>

PUBLIC namespace QuantumNEC::Architecture {
    PUBLIC class x86_64Architecture :
        public CPU::CPUManagement,
        public Driver::DriverManagement,
        public Interrupt::InterruptManagement
    {
    public:
        explicit( true ) x86_64Architecture( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config );

    public:
        virtual ~x86_64Architecture( VOID ) noexcept( true ) = default;
    };
}