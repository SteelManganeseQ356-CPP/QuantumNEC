#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt {
    constexpr CONST auto PIC_M_CTRL { 0x20 }; /* 主 PIC 的 IO 基址 */
    constexpr CONST auto PIC_S_CTRL { 0xA0 }; /* 从 PIC 的 IO 基址 */
    constexpr CONST auto PIC_M_COMMAND { PIC_M_CTRL };
    constexpr CONST auto PIC_M_DATA { ( PIC_M_CTRL + 1 ) }; /* 主数据 */
    constexpr CONST auto PIC_S_COMMAND { PIC_S_CTRL };
    constexpr CONST auto PIC_S_DATA { ( PIC_S_CTRL + 1 ) }; /* 从数据 */
    constexpr CONST auto PIC_EOI { 0x20 };                  /* 中断结束 */
    constexpr CONST auto ICW1 { 0x11 };                     /* 中断控制命令字PIC用于初始化 */
    constexpr CONST auto ICW1_ICW4 { 0x01 };                /* 表示ICW4将出现 */
    constexpr CONST auto ICW1_SINGLE { 0x02 };              /* 单（级联）模式 */
    constexpr CONST auto ICW1_INTERVAL4 { 0x04 };           /* 呼叫地址间隔4（8） */
    constexpr CONST auto ICW1_LEVEL { 0x08 };               /* 电平触发（边缘）模式 */
    constexpr CONST auto ICW1_INIT { 0x10 };                /* 初始化-必需！*/
    constexpr CONST auto ICW4_8086 { 0x01 };                /* 8086/88（MCS-80/85）模式 */
    constexpr CONST auto ICW4_AUTO { 0x02 };                /* 自动（正常）EOI */
    constexpr CONST auto ICW4_BUF_SLAVE { 0x08 };           /* 缓冲模式/从片 */
    constexpr CONST auto ICW4_BUF_MASTER { 0x0C };          /* 缓冲模式/主片 */
    constexpr CONST auto ICW4_SFNM { 0x10 };                /* 特殊完全嵌套（非） */
    constexpr CONST auto PIC_READ_IRR { 0x0a };             /* OCW3 irq准备好下一次CMD读取 */
    constexpr CONST auto PIC_READ_ISR { 0x0b };             /* OCW3 irq服务下一次CMD读取 */
    PUBLIC class PIC8259AManagement
    {
    private:
        using irq_t = Lib::Types::uint8_t;

    public:
        explicit( true ) PIC8259AManagement( VOID ) noexcept;

    public:
        virtual ~PIC8259AManagement( VOID ) noexcept;

    public:
        STATIC auto eoi( IN CONST irq_t irq ) -> VOID;
        STATIC auto disable_8259A_pic( VOID ) -> VOID;
        STATIC auto enable_8259A_pic( VOID ) -> VOID;
        STATIC auto irq_set_mask( IN irq_t irq ) -> VOID;
        STATIC auto irq_clear_mask( IN irq_t irq ) -> VOID;
        STATIC auto get_irr( void ) -> Lib::Types::uint16_t;
        STATIC auto get_isr( void ) -> Lib::Types::uint16_t;

    private:
        STATIC auto get_irq_reg( IN Lib::Types::int32_t ocw3 ) -> Lib::Types::uint16_t;
    };
}