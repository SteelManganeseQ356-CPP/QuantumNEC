#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Arch/x86_64/cpu/xdt.hpp>
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::CPU {
    /**
     * @brief CPU管理类
     */
    PUBLIC class CPUManagement :
        public InterruptDescriptorManagement,
        public GlobalSegmentDescriptorManagement

    {
    public:
        explicit( true ) CPUManagement( VOID ) noexcept;
        virtual ~CPUManagement( VOID ) noexcept = default;

    public:
        /**
         * @brief 查询处理器硬件设施信息
         */
        auto cpuid( IN Lib::Types::uint32_t mop, IN Lib::Types::uint32_t sop, IN Lib::Types::Ptr< Lib::Types::uint32_t > eax, IN Lib::Types::Ptr< Lib::Types::uint32_t > ebx, IN Lib::Types::Ptr< Lib::Types::uint32_t > ecx, IN Lib::Types::Ptr< Lib::Types::uint32_t > edx ) -> VOID;

    private:
        inline STATIC QuantumNEC::Architecture::CPU::GlobalSegmentDescriptor _used gdt[ Platform::GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT ][ Platform::SEGMENT_DESCRIPTOR_COUNT ] { };
        inline STATIC QuantumNEC::Architecture::CPU::InterruptDescriptor idt[ QuantumNEC::Architecture::Platform::INTERRUPT_DESCRIPTOR_COUNT ] { };
    };

    // cpuid
}