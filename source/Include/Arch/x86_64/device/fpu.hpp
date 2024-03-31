#pragma once
#include <Lib/Base/deflib.hpp>
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Architecture::Device {
    PUBLIC constexpr CONST auto REGISTERS_COUNT { 80 };
    PUBLIC class FPUDriverManagement
    {
    private:
        enum class CR0_COMMAND : Lib::Types::int64_t {
            MP = 1 << 1,      // Monitor Coprocessor
            EM = 1 << 2,      // Emulation 启用模拟，表示没有 FPU
            TS = 1 << 3,      // Task Switch 任务切换，延迟保存浮点环境
            ET = 1 << 4,      // Extension Type 保留
            NE = 1 << 5,      // Numeric Error 启用内部浮点错误报告
            WP = 1 << 16,     // Write Protect 写保护（禁止超级用户写入只读页）帮助写时复制
            AM = 1 << 18,     // Alignment Mask 对齐掩码
            NW = 1 << 29,     // Not Write-Through 不是直写
            CD = 1 << 30,     // Cache Disable 禁用内存缓冲
            PG = 1 << 31,     // Paging 启用分页
        };

    public:
        typedef struct
        {
            Lib::Types::uint16_t control;
            Lib::Types::uint16_t reserved1;
            Lib::Types::uint16_t status;
            Lib::Types::uint16_t reserved2;
            Lib::Types::uint16_t queue;
            Lib::Types::uint16_t reserved3;
            Lib::Types::uint32_t fip;
            Lib::Types::uint16_t fps;
            Lib::Types::uint16_t fop;
            Lib::Types::uint32_t fdp;
            Lib::Types::uint16_t fds;
            Lib::Types::uint16_t reserved4;
            Lib::Types::uint8_t registers[ REGISTERS_COUNT ];
        } _packed FPUFrame;

    public:
        explicit( TRUE ) FPUDriverManagement( VOID ) noexcept( TRUE );
        virtual ~FPUDriverManagement( VOID ) noexcept( TRUE ) = default;

    public:
        /**
         * @brief 刷新FPU
         */
        STATIC auto flush_fpu( VOID ) -> VOID;
        /**
         * @brief 检查fpu是否完好
         * @return 完好就是TRUE，不完好就是FALSE
         */
        STATIC auto check_fpu( VOID ) -> Lib::Types::BOOL;
        /**
         * @brief 开启FPU
         * @param task 当前进程
         */
        STATIC auto enable_fpu( IN Lib::Types::Ptr< VOID > task ) -> VOID;
        /**
         * @brief 关闭FPU
         * @param task 当前进程
         */
        STATIC auto disable_fpu( IN Lib::Types::Ptr< VOID > task ) -> VOID;

    private:
        inline STATIC Lib::Types::Ptr< VOID > last_fpu_task { };
    };
}
