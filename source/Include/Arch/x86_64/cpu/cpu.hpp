#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Arch/x86_64/cpu/xdt.hpp>
PUBLIC namespace QuantumNEC::Architecture::CPU {
    /**
     * @brief CPU管理类
     */
    PUBLIC class CPUManagement :
        InterruptDescriptorManagement,
        GlobalDescriptorManagement

    {
    public:
        explicit( true ) CPUManagement( VOID ) noexcept;
        virtual ~CPUManagement( VOID ) noexcept = default;

    public:
        /**
         * @brief 查询处理器硬件设施信息
         */
        auto cpuID( IN Lib::Types::uint32_t mop, IN Lib::Types::uint32_t sop, IN Lib::Types::Ptr< Lib::Types::uint32_t > eax, IN Lib::Types::Ptr< Lib::Types::uint32_t > ebx, IN Lib::Types::Ptr< Lib::Types::uint32_t > ecx, IN Lib::Types::Ptr< Lib::Types::uint32_t > edx ) -> VOID;

    private:
        inline STATIC QuantumNEC::Architecture::CPU::SegmentDescriptor gdt[ QuantumNEC::Architecture::Platform::GDT_MAX ] { };
        inline STATIC QuantumNEC::Architecture::CPU::InterruptDescriptor idt[ QuantumNEC::Architecture::Platform::IDT_MAX ] { };
    };

    // cpuid
}