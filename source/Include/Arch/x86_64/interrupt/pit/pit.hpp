#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#ifndef _INTERRUPT_HPP_
#error Do not include the header file. Please include interrupt.hpp
#endif
#pragma once
#ifndef _PIT_HPP_
#define _PIT_HPP_
#endif

#include <Arch/x86_64/interrupt/pit/8254.hpp>
#include <Arch/x86_64/interrupt/pit/hpet.hpp>
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::PIT {
    PUBLIC class PITManagement :
#ifndef APIC
        P8254Management,     // 初始化8253_pit
#else
        HpetManagement     // 初始化Hpet
#endif
    {
    public:
        /**
         * @brief 初始化 PIT 8253 or HEPT
         */
        explicit( true ) PITManagement( VOID ) noexcept;

    public:
        virtual ~PITManagement( VOID ) noexcept;

    private:
    };
}