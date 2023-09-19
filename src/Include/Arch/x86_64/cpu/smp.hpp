#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Arch/x86_64/platform/descriptor.hpp>
#include <Arch/x86_64/platform/global.hpp>
#include <Lib/Base/deflib.hpp>
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::CPU {
    /**
     * @brief 任务状态段描述符
    */
    PUBLIC typedef struct TaskStateSegmentDescriptor64
    {
        Lib::Types::uint32_t reserved1;
        Lib::Types::uint32_t rsp0_l;
        Lib::Types::uint32_t rsp0_h;
        Lib::Types::uint32_t rsp1_l;
        Lib::Types::uint32_t rsp1_h;
        Lib::Types::uint32_t rsp2_l;
        Lib::Types::uint32_t rsp2_h;
        Lib::Types::uint32_t reserved2;
        Lib::Types::uint32_t reserved3;
        Lib::Types::uint32_t ist1_l;
        Lib::Types::uint32_t ist1_h;
        Lib::Types::uint32_t ist2_l;
        Lib::Types::uint32_t ist2_h;
        Lib::Types::uint32_t ist3_l;
        Lib::Types::uint32_t ist3_h;
        Lib::Types::uint32_t ist4_l;
        Lib::Types::uint32_t ist4_h;
        Lib::Types::uint32_t ist5_l;
        Lib::Types::uint32_t ist5_h;
        Lib::Types::uint32_t ist6_l;
        Lib::Types::uint32_t ist6_h;
        Lib::Types::uint32_t ist7_l;
        Lib::Types::uint32_t ist7_h;
        Lib::Types::uint32_t reserved4;
        Lib::Types::uint32_t reserved5;
        Lib::Types::uint32_t io_map;
        auto set( IN Lib::Types::Ptr< TaskStateSegmentDescriptor64 > descriptor ) -> TaskStateSegmentDescriptor64 & {
            *this = *descriptor;
            return *this;
        }
    } _packed TaskStateSegmentDescriptor64;
    /**
     * @brief 任务状态段描述符管理
     */
    PUBLIC class TaskStateSegmentDescriptorManagement final
    {
    public:
        /* 任务状态段描述符属性 */
        enum class TaskStateSegmentDescriptorAttribute
        {
            AVAILABLE = 0x9,
            BUSY = 0xb
        };

    public:
        /**
         * @brief 初始化gdt里的tss
         * @param _gdt 要添加tss的全局描述符表
         */
        explicit( TRUE ) TaskStateSegmentDescriptorManagement( IN Lib::Types::Ptr< VOID > _gdt ) noexcept;
        virtual ~TaskStateSegmentDescriptorManagement( VOID ) noexcept = default;

    public:
        /**
         * @brief 设置TSS
         */
        auto set( VOID ) CONST->VOID;

    public:
        /**
         * @brief TSS描述符
         */
        STATIC TaskStateSegmentDescriptor64 tss /*[ Platform::TSS_MAX ]*/;
    };
    TaskStateSegmentDescriptor64 TaskStateSegmentDescriptorManagement::tss /*[ Platform::TSS_MAX ]*/ { };
}
