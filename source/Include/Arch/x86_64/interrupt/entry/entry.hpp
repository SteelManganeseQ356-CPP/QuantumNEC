#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#ifndef _INTERRUPT_HPP_
#error Do not include the header file. Please include interrupt.hpp
#endif
#pragma once
#ifndef _ENTRY_HPP_
#define _ENTRY_HPP_
#endif
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
#include <Arch/x86_64/interrupt/entry/time.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PUBLIC class InterruptEntryRegister :
        TimeEntry     // 时间中断(0x20号中断)入口注册
    {
    public:
        explicit( TRUE ) InterruptEntryRegister( VOID ) noexcept( TRUE );
        virtual ~InterruptEntryRegister( VOID ) noexcept( TRUE );

    private:
    };
}