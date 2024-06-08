#pragma once
#include "Lib/Base/deflib.hpp"
#include "Lib/Types/type_ptr.hpp"
#ifndef _PLATFORM_HPP_
#define _PLATFORM_HPP_
#endif
#include <Arch/x86_64/abi/builtins.hpp>
#include <Arch/x86_64/cpu/cpu.hpp>
#include <Arch/x86_64/cpu/xdt.hpp>
#include <Arch/x86_64/cpu/smp.hpp>
#include <Arch/x86_64/interrupt/interrupt.hpp>
#include <Arch/x86_64/device/device.hpp>
#include <Arch/x86_64/platform/global.hpp>
#include <Arch/x86_64/platform/syscall.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    PUBLIC class x86_64Architecture :
        // 架构组织
        public CPU::CPUManagement,
        public Device::DeviceManagement,
        public Interrupt::InterruptManagement,
        public Platform::SyscallManagement
    {
    public:
        explicit( true ) x86_64Architecture( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config );

    public:
        virtual ~x86_64Architecture( VOID ) noexcept( true ) = default;

    public:
        inline STATIC Lib::Types::Ptr< Lib::Types::byte_t > kernel_stack_space;
    };
}