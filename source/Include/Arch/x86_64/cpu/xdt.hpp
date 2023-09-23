#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Arch/x86_64/platform/descriptor.hpp>
#include <Arch/x86_64/platform/global.hpp>
#include <Arch/x86_64/interrupt/interrupt.hpp>
#include <Lib/STL/type_traits>
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>

PUBLIC namespace QuantumNEC::Architecture::CPU {
    constexpr CONST auto SEGFLAG_RW { Lib::Base::BIT( 1 ) };
    constexpr CONST auto SEGFLAG_EXCUTEABLE { Lib::Base::BIT( 3 ) };
    /* 可以指向 r15 寄存器在内存中的位置,
   以获取发生异常时寄存器信息.        */
    PUBLIC typedef struct
    {
        Platform::RegisterFrame regs;
        Lib::Types::uint64_t irq;
        Lib::Types::uint64_t errCode;
        Lib::Types::uint64_t rip;
        Lib::Types::uint64_t cs;
        Lib::Types::uint64_t rflags;
        Lib::Types::uint64_t rsp;
        Lib::Types::uint64_t ss;
    } _packed InterruptFrame;
    /**
     * @brief 中断描述符
     */
    PUBLIC typedef struct interruptDescriptor
    {
        Lib::Types::uint16_t LowOffset;    /* 偏移15~0 */
        Lib::Types::uint16_t selector;     /* 目标代码段选择子 */
        Lib::Types::uint8_t ist;           /* ist和空值 */
        Lib::Types::uint8_t attribute;     /* 属性 */
        Lib::Types::uint16_t middleOffset; /* 偏移31~16 */
        Lib::Types::uint32_t highOffset;   /* 偏移63~32 */
        Lib::Types::uint32_t reserved;     /* 保留 */
        auto set( IN Lib::Types::Ptr< interruptDescriptor > descriptor ) -> interruptDescriptor & {
            *this = *descriptor;
            return *this;
        }
    } _packed InterruptDescriptor;
    /**
     * @brief 中断描述符表管理器
     */
    PUBLIC class InterruptDescriptorManagement :
        Platform::DescriptorManagement< InterruptDescriptor >
    {
    private:
        enum class InterruptDescriptorAttribute {
            INTERRUPT = 0x8E,
            TRAP = 0x8F,
            SYSTEM = 0xEF,
            SYSTEM_INTERRUPT = 0xEE
        };

    public:
        /**
         * @brief 初始化中断描述符表
         * @param _descriptor 要绑定的中断描述符表
         * @param _num 要绑定的中断描述符数量
         */
        explicit( TRUE ) InterruptDescriptorManagement( IN Lib::Types::Ptr< InterruptDescriptor > _descriptor, IN Lib::Types::uint16_t _num ) noexcept;
        virtual ~InterruptDescriptorManagement( VOID ) noexcept = default;

    public:
        /**
         * @brief 载入IDT
         */
        virtual auto load( VOID ) const -> VOID override;
        /**
         * @brief 读取IDT
         * @return 中断描述表开头地址
         */
        virtual auto read( VOID ) const -> Lib::Types::Ptr< InterruptDescriptor > override;

    public:
        /**
         * @brief 设置中断描述符
         * @param index 中断号
         * @param entryPoint 中断入口函数头
         * @param selector 选择器
         * @param ist 状态
         * @param attributes 属性
         * @return 构建好的中断描述符
         */
        virtual auto makeDescriptor(
            IN CONST Lib::Types::uint16_t index,
            IN CONST Lib::Types::uint64_t entryPoint,
            IN CONST Lib::Types::uint16_t selector,
            IN CONST Lib::Types::uint8_t ist,
            IN CONST Lib::Types::uint8_t attributes ) -> Lib::Types::L_Ref< InterruptDescriptor > final;

    public:
        /**
         * @brief 输出寄存器状态
         * @param regs 寄存器数据
         */
        STATIC auto displayRegs( IN Lib::Types::Ptr< CONST Platform::RegisterFrame > regs ) -> VOID;
        /**
         * @brief 设置中断入口函数
         * @param nr 中断号
         * @param handle 要注册的入口函数指针
         */
        STATIC auto setHandle( IN CONST Lib::Types::uint8_t nr, IN Lib::Types::FuncPtr< VOID, Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > > handle ) -> VOID;

    public:
        /**
         * @brief 中断入口函数表
         */
        inline STATIC Lib::Types::FuncPtr< VOID, Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > > interruptEntryTable[ Platform::IDT_MAX ] { };
    };
    /**
     * @brief 段描述符
     */
    PUBLIC typedef struct SegmentDescriptor
    {
        Lib::Types::uint16_t limit_low;       // 0-15 limit1
        Lib::Types::uint16_t base_low;        // 16 - 31 base0
        Lib::Types::uint8_t base_mid;         // 32 - 39 base1
        Lib::Types::uint8_t access_right;     // 40 - 47 flag descType privilege isVaild
        Lib::Types::uint8_t limit_high;       // 48 - 55 limit1 usused
        Lib::Types::uint8_t base_high;        // 56 - 63 base2
        auto set( IN Lib::Types::Ptr< SegmentDescriptor > descriptor ) -> Lib::Types::L_Ref< SegmentDescriptor > {
            *this = *descriptor;
            return *this;
        }
    } _packed SegmentDescriptor;
    /**
     * @brief 任务状态描述符
     */
    PUBLIC typedef struct TaskStateDescriptor
    {
        Lib::Types::uint16_t limit0;
        Lib::Types::uint16_t base0;
        Lib::Types::uint8_t base1;
        Lib::Types::uint32_t flag : 4;
        Lib::Types::uint32_t descType : 1;
        Lib::Types::uint32_t privilege : 2;
        Lib::Types::BOOL isVaild : 1;
        Lib::Types::uint32_t limit1 : 4;
        Lib::Types::uint32_t usused : 4;
        Lib::Types::uint8_t base2;
        Lib::Types::uint32_t base3;
        Lib::Types::uint32_t reserved;
        auto set( IN Lib::Types::Ptr< TaskStateDescriptor > descriptor ) -> Lib::Types::L_Ref< TaskStateDescriptor > {
            *this = *descriptor;
            return *this;
        }
    } _packed TaskStateDescriptor;
    /**
     * @brief 全局描述符管理
     */
    PUBLIC class GlobalDescriptorManagement :
        Platform::DescriptorManagement< SegmentDescriptor >
    {
    public:
        /**
         * @brief 全局描述符状态
         */
        enum class GlobalDescriptorAttribute {
            SYSTEM = 0,
            NORMAL = 1
        };

    public:
        /**
         * @brief 初始化全局描述符
         * @param _descriptor 要绑定的全局描述符表
         * @param _num 要绑定的全局描述符数量
         */
        explicit( TRUE ) GlobalDescriptorManagement( IN Lib::Types::Ptr< SegmentDescriptor > _descriptor, IN Lib::Types::uint16_t _num ) noexcept;
        virtual ~GlobalDescriptorManagement( VOID ) noexcept = default;

    public:
        /**
         * @brief 载入GDT
         */
        virtual auto load( VOID ) CONST->VOID override;
        /**
         * @brief 读取GDT
         * @return 指向全局描述符表开头的地址
         */
        virtual auto read( VOID ) CONST->Lib::Types::Ptr< SegmentDescriptor > override;

    public:
        /**
         * @brief 为全局描述符制作段描述符
         * @param descriptor 要绑定的全局描述符
         * @param index 编号
         */
        STATIC auto makeDescriptor( IN Lib::Types::Ptr< SegmentDescriptor > descriptor, IN Lib::Types::uint8_t index, IN Lib::Types::uint32_t base, IN Lib::Types::uint32_t limit, IN Lib::Types::uint32_t access ) -> Lib::Types::L_Ref< SegmentDescriptor >;
        /**
         * @brief 为全局描述符制作任务状态描述符
         * @param descriptor 要绑定的全局描述符
         * @param index 编号
         */
        STATIC auto makeDescriptor( IN Lib::Types::Ptr< SegmentDescriptor > descriptor, IN Lib::Types::uint8_t index, IN Lib::Types::Ptr< VOID > base, IN Lib::Types::uint32_t limit, IN Lib::Types::uint8_t flag ) -> Lib::Types::L_Ref< TaskStateDescriptor >;
    };
}