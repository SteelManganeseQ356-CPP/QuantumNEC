#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#ifndef _INTERRUPT_HPP_
#error Do not include the header file. Please include interrupt.hpp
#endif
#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Driver/driver.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt {
    constexpr CONST auto IA32_APIC_BASE_MSR { 0x1B };
    constexpr CONST auto IA32_APIC_BASE_MSR_BSP { 1UL << 8U };     // 处理器是 BSP
    constexpr CONST auto IA32_APIC_BASE_MSR_ENABLE { 1UL << 11U };
    constexpr CONST auto IA32_APIC_BASE_MSR_BASE_ADDR_MSK { ~0xFFFUL };
    constexpr CONST auto LOCAL_APIC_ID { 0x0020 / 4 };          // 编号
    constexpr CONST auto LOCAL_APIC_VER { 0x0030 / 4 };         // 版本
    constexpr CONST auto LOCAL_APIC_TPR { 0x0080 / 4 };         // 任务优先级
    constexpr CONST auto LOCAL_APIC_EOI { 0x00B0 / 4 };         // 意向书
    constexpr CONST auto LOCAL_APIC_SVR { 0x00F0 / 4 };         // 杂散中断向量
    constexpr CONST auto LOCAL_APIC_ENABLE { 0x00000100 };      // 单位使能
    constexpr CONST auto LOCAL_APIC_ESR { 0x0280 / 4 };         // 错误状态
    constexpr CONST auto LOCAL_APIC_ICRLO { 0x0300 / 4 };       // 中断命令
    constexpr CONST auto LOCAL_APIC_INIT { 0x00000500 };        // 初始化/重置
    constexpr CONST auto LOCAL_APIC_STARTUP { 0x00000600 };     // 启动 IPI
    constexpr CONST auto LOCAL_APIC_DELIVS { 0x00001000 };      // 交货状态
    constexpr CONST auto LOCAL_APIC_ASSERT { 0x00004000 };      // 断言中断 （vs deassert）
    constexpr CONST auto LOCAL_APIC_DEASSERT { 0x00000000 };
    constexpr CONST auto LOCAL_APIC_LEVEL { 0x00008000 };     // 级别触发
    constexpr CONST auto LOCAL_APIC_BCAST { 0x00080000 };     // 发送给所有 APIC，包括自己。
    constexpr CONST auto LOCAL_APIC_BUSY { 0x00001000 };
    constexpr CONST auto LOCAL_APIC_FIXED { 0x00000000 };
    constexpr CONST auto LOCAL_APIC_ICRHI { 0x0310 / 4 };        // 中断命令 [63：32]
    constexpr CONST auto LOCAL_APIC_TIMER { 0x0320 / 4 };        // 局部向量表 0 （TIMER）
    constexpr CONST auto LOCAL_APIC_X1 { 0x0000000B };           // 将计数除以 1
    constexpr CONST auto LOCAL_APIC_PERIODIC { 0x00020000 };     // 周期的
    constexpr CONST auto LOCAL_APIC_PCINT { 0x0340 / 4 };        // 性能计数器 LVT
    constexpr CONST auto LOCAL_APIC_LINT0 { 0x0350 / 4 };        // 本地向量表 1 （LINT0）
    constexpr CONST auto LOCAL_APIC_LINT1 { 0x0360 / 4 };        // 本地向量表 2 （LINT1）
    constexpr CONST auto LOCAL_APIC_ERROR { 0x0370 / 4 };        // 本地向量表 3 （ERROR）
    constexpr CONST auto LOCAL_APIC_MASKED { 0x00010000 };       // 中断屏蔽
    constexpr CONST auto LOCAL_APIC_TICR { 0x0380 / 4 };         // 计时器初始计数
    constexpr CONST auto LOCAL_APIC_TCCR { 0x0390 / 4 };         // 定时器电流计数
    constexpr CONST auto LOCAL_APIC_TDCR { 0x03E0 / 4 };         // 定时器分频配置
    constexpr CONST auto IOAPIC_REG_ID { 0x00 };
    constexpr CONST auto IOAPIC_REG_VER { 0x01 };
    constexpr CONST auto IOAPIC_REG_TABLE { 0x10 };
    /* 重定向表从 REG_TABLE 开始，使用两个寄存器来配置每个中断。一对中的第一个（低电平）寄存器包含配置位。 第二个（高）寄存器包含一个位掩码，告诉哪个CPU 可以提供该中断。 */
    constexpr CONST auto INT_DISABLED { 0x00010000 };      // 中断已禁用
    constexpr CONST auto INT_LEVEL { 0x00008000 };         // 电平触发（与边沿-）
    constexpr CONST auto INT_ACTIVELOW { 0x00002000 };     // 低电平有效（与高电平相比）
    constexpr CONST auto INT_LOGICAL { 0x00000800 };       // 目标为 CPU ID（与 APIC ID 相对）
    PUBLIC class ApicManagement
    {
    public:
        friend Driver::AcpiManagement;

        using irq_t = Lib::Types::uint8_t;

        struct Apic
        {
            Lib::Types::uint64_t local_apic_address;
            Lib::Types::uint32_t io_apic_address;
            Lib::Types::Ptr< VOID > io_apic_index_address;
            Lib::Types::Ptr< VOID > io_apic_EOI_address;
            Lib::Types::Ptr< VOID > io_apic_data_address;
            Lib::Types::uint8_t core_count;
            Lib::Types::uint8_t local_apic_ID[ 256 ];
        };
        volatile struct ioapic *ioapic;

        enum class ApicType {
            IO_APIC = 0,
            LOCAL_APIC = 1
        };

    public:
        explicit( TRUE ) ApicManagement( VOID ) noexcept;
        virtual ~ApicManagement( VOID ) noexcept;

    public:
        STATIC auto write_apic( IN Lib::Types::uint16_t index, IN Lib::Types::uint32_t value, IN ApicType type ) -> VOID;
        STATIC auto read_apic( IN Lib::Types::uint16_t index, IN ApicType type ) -> Lib::Types::uint64_t;
        STATIC auto enable_ioapic( IN Lib::Types::uint64_t pin, IN Lib::Types::uint64_t vector ) -> VOID;
        STATIC auto eoi( IN CONST irq_t irq ) -> VOID;

        /**
         * @brief 检测CPU是否支持APIC
         * @return 如果CPU支持APIC并且本地APIC未在MSR中禁用，则返回 TRUE 值
         */
        STATIC auto check_apic( VOID ) -> Lib::Types::BOOL;

    private:
        inline STATIC Apic apic;
    };
}