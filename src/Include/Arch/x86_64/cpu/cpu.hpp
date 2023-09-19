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
         * @brief 硬件设施信息
         */
        auto cpuInfo( VOID ) -> VOID;

    public:
        STATIC auto invlpg( IN Lib::Types::Ptr< VOID > address ) -> VOID;
    };

    // cpuid
}