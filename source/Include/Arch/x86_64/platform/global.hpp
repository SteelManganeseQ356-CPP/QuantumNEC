#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Lib/Types/type_base.hpp>
#include <Lib/Types/type_int.hpp>
PUBLIC namespace QuantumNEC::Architecture::Platform {
    PUBLIC typedef struct
    {
        Lib::Types::uint64_t rax;
        Lib::Types::uint64_t rbx;
        Lib::Types::uint64_t rcx;
        Lib::Types::uint64_t rdx;
        Lib::Types::uint64_t rsi;
        Lib::Types::uint64_t rdi;
        Lib::Types::uint64_t rbp;
        Lib::Types::uint64_t r8;
        Lib::Types::uint64_t r9;
        Lib::Types::uint64_t r10;
        Lib::Types::uint64_t r11;
        Lib::Types::uint64_t r12;
        Lib::Types::uint64_t r13;
        Lib::Types::uint64_t r14;
        Lib::Types::uint64_t r15;
    } _packed RegisterFrame;
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
    PUBLIC constexpr const auto RPL0 { 0x0 };
    PUBLIC constexpr const auto RPL1 { 0x1 };
    PUBLIC constexpr const auto RPL2 { 0x2 };
    PUBLIC constexpr const auto RPL3 { 0x3 };
    PUBLIC constexpr const auto TI_GDT { 0x0 };
    PUBLIC constexpr const auto TILDT { 0x4 };
    PUBLIC constexpr const auto SELECTOR_CODE64_K { ( 1 << 3 ) | TI_GDT | RPL0 }; /* 代码段 */
    PUBLIC constexpr const auto SELECTOR_DATA64_K { ( 2 << 3 ) | TI_GDT | RPL0 }; /* 数据段 */
    PUBLIC constexpr const auto SELECTOR_TSS { ( 9 << 3 ) | TI_GDT | RPL0 };      /* TSS段 */
    PUBLIC constexpr const auto SELECTOR_CODE64_U { ( 3 << 3 ) | TI_GDT | RPL3 }; /* 用户代码段 */
    PUBLIC constexpr const auto SELECTOR_DATA64_U { ( 4 << 3 ) | TI_GDT | RPL3 }; /* 用户数据段 */
    PUBLIC constexpr const auto SELECTOR_CODE32_K { ( 7 << 3 ) | TI_GDT | RPL0 };
    /* 代码段 */
    PUBLIC constexpr const auto SELECTOR_DATA32_K { ( 8 << 3 ) | TI_GDT | RPL0 }; /* 数据段 */

    /** @brief -------------GDT描述符属性 **/

    PUBLIC constexpr const auto KERNEL_PAGE_DIR_TABLE_POS { (unsigned long long)0x5f9000 };
    PUBLIC constexpr const auto KERNEL_VMA_BASE { 0xffff800000000000 };

    PUBLIC constexpr inline auto KADDR_P2V( const auto &addr ) {
        return addr + KERNEL_VMA_BASE;
    }
    PUBLIC constexpr inline auto KADDR_V2P( const auto &addr ) {
        return addr - KERNEL_VMA_BASE;
    }
    /** @brief -------------TSS描述符属性 **/

    PUBLIC constexpr const auto TSS_D_0 { 0 };
    PUBLIC constexpr const auto AR_TSS32 { AR_G_4K | TSS_D_0 | AR_L | AR_AVL | AR_P | AR_DPL_0 | AR_S_SYS | AR_TYPE_TSS };
    PUBLIC constexpr const auto AR_TSS64 { AR_G_4K | TSS_D_0 | AR_L | AR_AVL | AR_P | AR_DPL_0 | AR_S_SYS | AR_TYPE_TSS };
    PUBLIC constexpr const auto EFLAGS_MBS { 1 << 1 };
    PUBLIC constexpr const auto EFLAGS_IF_1 { 1 << 9 };
    PUBLIC constexpr const auto EFLAGS_IF_0 { 0 << 9 };
    PUBLIC constexpr const auto EFLAGS_IOPL_0 { 0 << 12 };

    /** @brief -------------IDT描述符属性 **/

    PUBLIC constexpr const auto IDT_DESC_P { 1 };
    PUBLIC constexpr const auto IDT_DESC_DPL0 { 0 };
    PUBLIC constexpr const auto IDT_DESC_DPL3 { 3 };
    PUBLIC constexpr const auto IDT_DESC_32_TYPE { 0xE };     // 32 位的门
    PUBLIC constexpr const auto IDT_DESC_16_TYPE { 0x6 };     // 16 位的门,不会用到
    PUBLIC constexpr const auto IDT_DESC_ATTR_DPL0 { ( IDT_DESC_P << 7 ) + ( IDT_DESC_DPL0 << 5 ) + IDT_DESC_32_TYPE };
    PUBLIC constexpr const auto IDT_DESC_ATTR_DPL3 { ( IDT_DESC_P << 7 ) + ( IDT_DESC_DPL3 << 5 ) + IDT_DESC_32_TYPE };

    /** @brief -------------三种描述符数量 **/

    PUBLIC constexpr const auto IDT_MAX { 256 };
    PUBLIC constexpr const auto GDT_MAX { 128 };
    PUBLIC constexpr const auto TSS_MAX { 8 };
}     // namespace QuantumNEC::Architecture::Platform