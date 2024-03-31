#pragma once
#ifndef _PLATFORM_HPP_
#define _PLATFORM_HPP_
#endif
#include <Arch/x86_64/abi/builtins.hpp>
#include <Arch/x86_64/abi/cxxabi.hpp>
#include <Arch/x86_64/cpu/cpu.hpp>
#include <Arch/x86_64/cpu/xdt.hpp>
#include <Arch/x86_64/cpu/smp.hpp>
#include <Arch/x86_64/interrupt/interrupt.hpp>
#include <Arch/x86_64/device/device.hpp>
#include <Arch/x86_64/platform/global.hpp>
#include <Arch/x86_64/platform/task_utils.hpp>
#include <Utils/asm.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    PUBLIC class x86_64Architecture :
        // 架构组织
        public CPU::CPUManagement,
        public Device::DeviceManagement,
        public Interrupt::InterruptManagement
    {
    public:
        explicit( true ) x86_64Architecture( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config );

    public:
        virtual ~x86_64Architecture( VOID ) noexcept( true ) = default;

    public:
        STATIC auto flush_tlb( VOID ) -> VOID {
            Lib::Types::uint64_t tmp { };
            ASM(
                "MOVQ	%%CR3,	%0\n\t"
                "MOVQ	%0,	%%CR3 \n\t"
                : "=r"( tmp )
                :
                : "memory" );
        }
        STATIC auto asm_switch_to( IN OUT Lib::Types::Ptr< VOID > current, IN Lib::Types::Ptr< VOID > next ) -> VOID {
            ASM(
                "PUSHQ %1\n\t"
                "PUSHQ %0\n\t"
                "LEAQ _asm_switch_to(%%RIP), %%RAX\n\t"
                "CALLQ *%%RAX\n\t"
                :
                : "g"( current ),
                  "g"( next )
                : );
        };
    };
}