#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#ifndef _INTERRUPT_HPP_
#error Do not include the header file. Please include interrupt.hpp
#endif
#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt {
    constexpr CONST auto PIC_M_CTRL { 0x20 }; /* 主 PIC 的 IO 基址 */
    constexpr CONST auto PIC_S_CTRL { 0xA0 }; /* 从 PIC 的 IO 基址 */
    constexpr CONST auto PIC1_COMMAND { PIC_M_CTRL };
    constexpr CONST auto PIC1_DATA { ( PIC_M_CTRL + 1 ) }; /* 主数据 */
    constexpr CONST auto PIC2_COMMAND { PIC_S_CTRL };
    constexpr CONST auto PIC2_DATA { ( PIC_S_CTRL + 1 ) }; /* 从数据 */
    constexpr CONST auto PIC_EOI { 0x20 };                 /* 中断结束 */
    constexpr CONST auto ICW1 { 0x11 };                    /* 中断控制命令字PIC用于初始化 */
    PUBLIC class Pic8259aManagement
    {
    private:
        using irq_t = Lib::Types::uint8_t;

    public:
        explicit( true ) Pic8259aManagement( VOID ) noexcept( true );

    public:
        virtual ~Pic8259aManagement( VOID ) noexcept( true );

    public:
        auto eoi( IN CONST irq_t irq ) -> VOID;

    private:
    };
}