#pragma once
#include <Lib/Types/Uefi.hpp>

PUBLIC namespace QuantumNEC::Architecture {
    PUBLIC struct ControlRegisterFrame
    {
        Lib::Types::uint64_t cr0;
        Lib::Types::uint64_t cr1;
        Lib::Types::uint64_t cr2;
        Lib::Types::uint64_t cr3;
        Lib::Types::uint64_t cr4;
        Lib::Types::uint64_t cr5;
        Lib::Types::uint64_t cr6;
        Lib::Types::uint64_t cr7;
        Lib::Types::uint64_t cr8;
        Lib::Types::uint64_t cr9;
        Lib::Types::uint64_t cr10;
        Lib::Types::uint64_t cr11;
        Lib::Types::uint64_t cr12;
        Lib::Types::uint64_t cr13;
        Lib::Types::uint64_t cr14;
        Lib::Types::uint64_t cr15;
        explicit ControlRegisterFrame( VOID ) noexcept;
        ~ControlRegisterFrame( VOID ) noexcept;
        // CR1，CR5~CR7, CR9~CR15为保留，无法访问
        // CR8目前不知道咋访问
    } _packed;
    PUBLIC struct GeneralRegisterFrame
    {
        Lib::Types::uint64_t rax;
        Lib::Types::uint64_t rbx;
        Lib::Types::uint64_t rcx;
        Lib::Types::uint64_t rdx;
        Lib::Types::uint64_t rbp;
        Lib::Types::uint64_t rsi;
        Lib::Types::uint64_t rdi;
        Lib::Types::uint64_t r8;
        Lib::Types::uint64_t r9;
        Lib::Types::uint64_t r10;
        Lib::Types::uint64_t r11;
        Lib::Types::uint64_t r12;
        Lib::Types::uint64_t r13;
        Lib::Types::uint64_t r14;
        Lib::Types::uint64_t r15;
    } _packed;
    PUBLIC struct SegmentRegisterFrame
    {
        Lib::Types::uint64_t ds;
        Lib::Types::uint64_t es;
        Lib::Types::uint64_t fs;
        Lib::Types::uint64_t gs;
    } _packed;
    PUBLIC struct RegisterFrame :
        SegmentRegisterFrame,
        GeneralRegisterFrame
    {
    } _packed;
    /* ------------------------------------------------------------------------------------ *
     * GDT内部：
     * n << 3 意味着 n * 2 ^ 3 = n * 8
     * 0x00 (0 << 3)    空描述符，保留
     * 0x08 (1 << 3)    64位内核代码段
     * 0x10 (2 << 3)    64位内核数据段
     * 0x18 (3 << 3)    32位用户代码段
     * 0x20 (4 << 3)    32位用户数据段
     * 0x28 (5 << 3)    64位用户代码段
     * 0x30 (6 << 3)    64位用户数据段
     * 0x38 (7 << 3)    32位内核代码段
     * 0x40 (8 << 3)    32位内核数据段
     * 0x48 (9 << 3)    空描述符，保留
     * 0x50 (10 << 3)   64位任务状态段低地址
     * 0x58 (11 << 3)   64位任务状态段高地址
     * 0x60 ~ 0x2000    保留，扩展用的
     * ------------------------------------------------------------------------------------ */

    PUBLIC constexpr const auto AR_G_4K { 0x8000 };
    PUBLIC constexpr const auto AR_D_32 { 0x4000 };
    PUBLIC constexpr const auto AR_D_64 { 0x0000 };
    PUBLIC constexpr const auto AR_L { 0x0000 };
    PUBLIC constexpr const auto AR_L_64 { 0x2000 };
    PUBLIC constexpr const auto AR_AVL { 0x0000 };
    PUBLIC constexpr const auto AR_P { 0x0080 };
    PUBLIC constexpr const auto AR_DPL_0 { 0x0000 };
    PUBLIC constexpr const auto AR_DPL_1 { 0x0020 };
    PUBLIC constexpr const auto AR_DPL_2 { 0x0040 };
    PUBLIC constexpr const auto AR_DPL_3 { 0x0060 };
    PUBLIC constexpr const auto AR_S_CODE { 0x0010 };
    PUBLIC constexpr const auto AR_S_DATA { 0x0010 };
    PUBLIC constexpr const auto AR_S_SYS { 0x0000 };
    PUBLIC constexpr const auto AR_TYPE_CODE { 0x0008 }; /* 可执行,非一致,不可读 */
    PUBLIC constexpr const auto AR_TYPE_DATA { 0x0002 }; /* 不可执行,向上拓展,可写 */
    PUBLIC constexpr const auto AR_TYPE_TSS { 0x0009 };
    PUBLIC constexpr const auto AR_CODE32 { AR_G_4K | AR_D_32 | AR_L | AR_AVL | AR_P | AR_DPL_0 | AR_S_CODE | AR_TYPE_CODE };
    PUBLIC constexpr const auto AR_CODE32_DPL3 { AR_G_4K | AR_D_32 | AR_L | AR_AVL | AR_P | AR_DPL_3 | AR_S_CODE | AR_TYPE_CODE };
    PUBLIC constexpr const auto AR_DATA32 { AR_G_4K | AR_D_32 | AR_L | AR_AVL | AR_P | AR_DPL_0 | AR_S_DATA | AR_TYPE_DATA };
    PUBLIC constexpr const auto AR_DATA32_DPL3 { AR_G_4K | AR_D_32 | AR_L | AR_AVL | AR_P | AR_DPL_3 | AR_S_DATA | AR_TYPE_DATA };
    PUBLIC constexpr const auto AR_CODE64 { AR_G_4K | AR_D_64 | AR_L_64 | AR_AVL | AR_P | AR_DPL_0 | AR_S_CODE | AR_TYPE_CODE };
    PUBLIC constexpr const auto AR_CODE64_DPL3 { AR_G_4K | AR_D_64 | AR_L_64 | AR_AVL | AR_P | AR_DPL_3 | AR_S_CODE | AR_TYPE_CODE };
    PUBLIC constexpr const auto AR_DATA64 { AR_G_4K | AR_D_64 | AR_L_64 | AR_AVL | AR_P | AR_DPL_0 | AR_S_DATA | AR_TYPE_DATA };
    PUBLIC constexpr const auto AR_DATA64_DPL3 { AR_G_4K | AR_D_64 | AR_L_64 | AR_AVL | AR_P | AR_DPL_3 | AR_S_DATA | AR_TYPE_DATA };
    PUBLIC constexpr const auto TSS_D_0 { 0 };
    PUBLIC constexpr const auto AR_TSS64 { AR_G_4K | TSS_D_0 | AR_L | AR_AVL | AR_P | AR_DPL_0 | AR_S_SYS | AR_TYPE_TSS };
    PUBLIC constexpr const auto AR_TSS32 { AR_G_4K | TSS_D_0 | AR_L | AR_AVL | AR_P | AR_DPL_0 | AR_S_SYS | AR_TYPE_TSS };
    PUBLIC constexpr const auto RPL0 { 0x0 };
    PUBLIC constexpr const auto RPL1 { 0x1 };
    PUBLIC constexpr const auto RPL2 { 0x2 };
    PUBLIC constexpr const auto RPL3 { 0x3 };
    PUBLIC constexpr const auto TI_GDT { 0x0 };
    PUBLIC constexpr const auto TILDT { 0x4 };
    PUBLIC constexpr const auto SELECTOR_CODE64_KERNEL { ( 1 << 3 ) | TI_GDT | RPL0 }; /* 64位内核代码段 */
    PUBLIC constexpr const auto SELECTOR_DATA64_KERNEL { ( 2 << 3 ) | TI_GDT | RPL0 }; /* 64位内核数据段 */
    PUBLIC constexpr const auto SELECTOR_CODE64_USER { ( 3 << 3 ) | TI_GDT | RPL3 };   /* 64位用户代码段 */
    PUBLIC constexpr const auto SELECTOR_DATA64_USER { ( 4 << 3 ) | TI_GDT | RPL3 };   /* 64位用户数据段 */
    PUBLIC constexpr const auto SELECTOR_TSS { ( 10 << 3 ) | TI_GDT | RPL0 };          /* TSS段 */

    /* RFLAGS */

    PUBLIC constexpr const auto RFLAGS_MBS { 1 << 1 };
    PUBLIC constexpr const auto RFLAGS_IF_1 { 1 << 9 };
    PUBLIC constexpr const auto RFLAGS_IF_0 { 0 << 9 };
    PUBLIC constexpr const auto RFLAGS_IOPL_0 { 0 << 12 };
    PUBLIC constexpr const auto RFLAGS_IOPL_3 { 3 << 12 };

    /* ------------------------------------------------------------------------------------
     * IDT内部：
     * 0 ~ 20   异常入口
        - 2 - NMI中断
        - 9 - 协处理器错误
        - 15 - intel保留
        - 以上皆为无用处异常，不可能产生
     * 21 ~ 26  保留
     * 27 ~ 29  异常入口
     * 30 ~ 256 中断入口
     * ------------------------------------------------------------------------------------ */

    PUBLIC constexpr const auto IDT_DESC_P { 1 };
    PUBLIC constexpr const auto IDT_DESC_DPL0 { 0 };
    PUBLIC constexpr const auto IDT_DESC_DPL3 { 3 };
    PUBLIC constexpr const auto IDT_DESC_64_TYPE { 0xF };     // 64 位的门
    PUBLIC constexpr const auto IDT_DESC_32_TYPE { 0xE };     // 32 位的门
    PUBLIC constexpr const auto IDT_DESC_16_TYPE { 0x6 };     // 16 位的门

    PUBLIC constexpr const auto INTERRUPT_DESCRIPTOR_COUNT { 256 };
    PUBLIC constexpr const auto GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT { 32 };
    PUBLIC constexpr const auto SEGMENT_DESCRIPTOR_COUNT { 8192 };
    PUBLIC constexpr const auto TASK_STATE_SEGMENT_DESCRIPTOR_COUNT { GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT };
    PUBLIC constexpr const auto LOCAL_DESCRIPTOR_COUNT { GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT * SEGMENT_DESCRIPTOR_COUNT - TASK_STATE_SEGMENT_DESCRIPTOR_COUNT };

    /** @brief -------------最大CORE数量 **/
    PUBLIC constexpr const auto CORE_COUNT { GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT };

    PUBLIC constexpr const auto IDT_ENTRY_IRQ_0 { 0x20 };
    PUBLIC constexpr const auto SYSTEM_CALL_INTERRUPTS_INDEX { 0x80 };
    PUBLIC constexpr const auto IRQ_CLOCK { IDT_ENTRY_IRQ_0 };
    PUBLIC constexpr const auto IRQ_KEYBOARD { IDT_ENTRY_IRQ_0 + 0x01 };
    PUBLIC constexpr const auto IRQ_CMOS_RTC { IDT_ENTRY_IRQ_0 + 0x08 };
    PUBLIC constexpr const auto IRQ_LOCAL_APIC_SPURIOUS { IDT_ENTRY_IRQ_0 + 0x1F };
    PUBLIC constexpr const auto IRQ_LOCAL_APIC_ERROR { IDT_ENTRY_IRQ_0 + 0x13 };

}     // namespace QuantumNEC::Architecture::Platform