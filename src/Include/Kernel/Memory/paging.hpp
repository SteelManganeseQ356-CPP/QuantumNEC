#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>

PUBLIC namespace QuantumNEC::Kernel::Memory {
    PUBLIC constexpr CONST auto PAGE_SIZE { 0x200000 };
    PUBLIC constexpr CONST auto PAGE_P { 1 };
    PUBLIC constexpr CONST auto PAGE_RW_R { 0 };
    PUBLIC constexpr CONST auto PAGE_RW_W { 2 };
    PUBLIC constexpr CONST auto PAGE_US_S { 0 };
    PUBLIC constexpr CONST auto PAGE_US_U { 4 };
    PUBLIC constexpr CONST auto PAGE_SIZE_2M { 128 };
    // 内存块种类的数量

    PUBLIC constexpr CONST auto KERNEL_HEAPV { 0xffff8a0000000000ULL };
    PUBLIC constexpr CONST auto KERNEL_PV { 0xfffff00000000000ULL };
    PUBLIC constexpr CONST auto KERNEL_FB { 0xffff000000000000ULL };
    PUBLIC constexpr CONST auto PAGE_OFFSET { 0xffff800000000000ULL };
    PUBLIC constexpr CONST auto TASK_SIZE { 0x00007fffffffffffULL };
    PUBLIC constexpr CONST auto PAGE_GDT_SHIFT { 39 };
    PUBLIC constexpr CONST auto PAGE_1G_SHIFT { 30 };
    PUBLIC constexpr CONST auto PAGE_2M_SHIFT { 21 };
    PUBLIC constexpr CONST auto PAGE_4K_SHIFT { 12 };
    PUBLIC constexpr CONST auto PAGE_2M_SIZE { ( 1UL << PAGE_2M_SHIFT ) };
    PUBLIC constexpr CONST auto PAGE_4K_SIZE { ( 1UL << PAGE_4K_SHIFT ) };
    PUBLIC constexpr CONST auto PAGE_2M_MASK { ( ~( PAGE_2M_SIZE - 1 ) ) };
    PUBLIC constexpr CONST auto PAGE_4K_MASK { ( ~( PAGE_4K_SIZE - 1 ) ) };
    PUBLIC inline constexpr auto Page2MAligned( auto addr ) {
        return ( ( (unsigned long)( addr ) + PAGE_2M_SIZE - 1 ) & PAGE_2M_MASK );
    };
    PUBLIC inline constexpr auto Page4KAligned( auto addr ) {
        return ( ( (unsigned long)( addr ) + PAGE_4K_SIZE - 1 ) & PAGE_4K_MASK );
    };

    PUBLIC constexpr CONST auto MEMORY_ALLOCATE_DESCRIPTOR { 2048 };
    PUBLIC constexpr CONST auto PAGE_XD { 1UL << 63 };
    PUBLIC constexpr CONST auto PAGE_PAT { 1UL << 12 };
    PUBLIC constexpr CONST auto PAGE_Global { 1UL << 8 };
    PUBLIC constexpr CONST auto PAGE_PS { 1UL << 7 };
    PUBLIC constexpr CONST auto PAGE_Dirty { 1UL << 6 };
    PUBLIC constexpr CONST auto PAGE_Accessed { 1UL << 5 };
    PUBLIC constexpr CONST auto PAGE_PCD { 1UL << 4 };
    PUBLIC constexpr CONST auto PAGE_PWT { 1UL << 3 };
    PUBLIC constexpr CONST auto PAGE_U_S { 1UL << 2 };
    PUBLIC constexpr CONST auto PAGE_R_W { 1UL << 1 };
    PUBLIC constexpr CONST auto PAGE_Present { 1UL << 0 };
    PUBLIC constexpr CONST auto PAGE_KERNEL_GDT { PAGE_R_W | PAGE_Present };
    PUBLIC constexpr CONST auto PAGE_KERNEL_Dir { PAGE_R_W | PAGE_Present };
    PUBLIC constexpr CONST auto PAGE_KERNEL_Page { PAGE_PS | PAGE_R_W | PAGE_Present };
    PUBLIC constexpr CONST auto PAGE_USER_GDT { PAGE_U_S | PAGE_R_W | PAGE_Present };
    PUBLIC constexpr CONST auto PAGE_USER_Dir { PAGE_U_S | PAGE_R_W | PAGE_Present };
    PUBLIC constexpr CONST auto PAGE_USER_Page { PAGE_PS | PAGE_U_S | PAGE_R_W | PAGE_Present };
    PUBLIC constexpr CONST auto PAGE_DESCRIPTOR_ENTRY_COUNT { 0x10000 };
    PUBLIC constexpr auto divRoundUp( auto X, auto STEP ) {
        return ( ( ( X ) + ( STEP - 1 ) ) / STEP );
    }
    PUBLIC inline constexpr auto Phy_To_Virt( auto address )->Lib::Types::Ptr< VOID > {
        return ( Lib::Types::Ptr< VOID >( ( QuantumNEC::Lib::Types::uint64_t )(address)-PAGE_OFFSET ) );
    }
    PUBLIC inline constexpr auto Virt_To_Phy( auto address )->Lib::Types::Ptr< VOID > {
        return ( Lib::Types::Ptr< VOID >( ( QuantumNEC::Lib::Types::uint64_t )( address ) + PAGE_OFFSET ) );
    }

    PUBLIC class PageMemoryManagement
    {
    private:
        enum class MemoryPageAttribute {
            FREE_MEMORY = 1,          // 可用内存
            RESERVED_MEMORY = 2,      // 保留(不可用)
            ACPI_MEMORY = 3,          // ACPI可回收内存
            ACPI_MEMORY_NVS = 4,      // ACPI NVS内存
            UNUSEABLE_MEMORY = 5,     // 不可用的内存
            UNDEF_MEMORY = 6,         // 未定义
        };

    public:
        explicit PageMemoryManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true );

    public:
        virtual ~PageMemoryManagement( VOID ) noexcept( true ) = default;

    public:
        STATIC auto malloc( IN Lib::Types::size_t ) -> Lib::Types::Ptr< VOID >;
        STATIC auto free( IN Lib::Types::Ptr< VOID >, IN Lib::Types::size_t ) -> VOID;
    };

    PUBLIC inline constexpr Lib::Types::size_t operator""_KB( IN CONST Lib::Types::size_t kib ) {
        return kib * 1024;
    }

    PUBLIC inline constexpr Lib::Types::size_t operator""_MB( IN CONST Lib::Types::size_t mib ) {
        return mib * 1024_KB;
    }

    PUBLIC inline constexpr Lib::Types::size_t operator""_GB( IN CONST Lib::Types::size_t gib ) {
        return gib * 1024_MB;
    }
}