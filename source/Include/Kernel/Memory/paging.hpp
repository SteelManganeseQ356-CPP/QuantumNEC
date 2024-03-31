#pragma once
#include <Lib/Base/bitmap.hpp>
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Kernel::Memory {
    PUBLIC constexpr CONST auto PAGE_SIZE { 0x200000 };
    PUBLIC constexpr CONST auto KERNEL_BASE_ADDRESS { 0xffff800000000000ULL };
    PUBLIC constexpr CONST auto KERNEL_PHYSICAL_ADDRESS { 0x100000ULL };
    PUBLIC constexpr CONST auto KERNEL_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_PHYSICAL_ADDRESS };
    PUBLIC constexpr CONST auto KERNEL_VRAM_PHYSICAL_ADDRESS { 0x800000 };
    PUBLIC constexpr CONST auto KERNEL_VRAM_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_VRAM_PHYSICAL_ADDRESS };
    PUBLIC constexpr CONST auto KERNEL_FONT_MEMORY_PHYSICAL_ADDRESS { 0x0a00000 };
    PUBLIC constexpr CONST auto KERNEL_FONT_MEMORY_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_FONT_MEMORY_PHYSICAL_ADDRESS };
    PUBLIC constexpr CONST auto KERNEL_FREE_MEMORY_PHYSICAL_ADDRESS { 0x2a00000 };
    PUBLIC constexpr CONST auto KERNEL_FREE_MEMORY_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_FREE_MEMORY_PHYSICAL_ADDRESS };
    PUBLIC constexpr CONST auto KERNEL_TASK_PCB_PHYSICAL_ADDRESS { 0x0200000 };
    PUBLIC constexpr CONST auto KERNEL_TASK_PCB_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_TASK_PCB_PHYSICAL_ADDRESS };
    PUBLIC constexpr CONST auto KERNEL_PAGE_DIRECTORY_PHYSICAL_ADDRESS { 0x0600000 };
    PUBLIC constexpr CONST auto KERNEL_PAGE_DIRECTORY_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_PAGE_DIRECTORY_PHYSICAL_ADDRESS };
    PUBLIC constexpr CONST auto KERNEL_I_O_APIC_PHYSICAL_ADDRESS { 0x0210000 };
    PUBLIC constexpr CONST auto KERNEL_I_O_APIC_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_I_O_APIC_PHYSICAL_ADDRESS };
    PUBLIC constexpr CONST auto KERNEL_LOCAL_APIC_PHYSICAL_ADDRESS { 0x0211000 };
    PUBLIC constexpr CONST auto KERNEL_LOCAL_APIC_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_LOCAL_APIC_PHYSICAL_ADDRESS };
    PUBLIC constexpr CONST auto KERNEL_PAGE_TABLE_RESERVED_PHYSICAL_ADDRESS { 0x0212000 };
    PUBLIC constexpr CONST auto KERNEL_PAGE_TABLE_RESERVED_VIRTUAL_ADDRESS { KERNEL_BASE_ADDRESS + KERNEL_PAGE_TABLE_RESERVED_PHYSICAL_ADDRESS };
    /*
     * 内核层 ： 00000000 00000000 -> FFFFFFFF FFFFFFFF
     * 应用层 ： 00000000 02A00000 -> 00007FFF FFFFFFFF
     */
    PUBLIC constexpr CONST auto KERNEL_STACK_VIRTUAL_START_ADDRESS { 0x0000000000000000ULL };
    PUBLIC constexpr CONST auto KERNEL_STACK_VIRTUAL_END_ADDRESS { 0xffffffffffffffffULL };
    PUBLIC constexpr CONST auto USER_STACK_VIRTUAL_START_ADDRESS { 0x0000000002A00000ULL };
    PUBLIC constexpr CONST auto USER_STACK_VIRTUAL_END_ADDRESS { 0x00007FFFFFFFFFFFULL };

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
    PUBLIC constexpr CONST auto MEMORY_PAGE_DESCRIPTOR { 2048 };
    PUBLIC constexpr CONST auto PAGE_XD { 1UL << 63 };
    PUBLIC constexpr CONST auto PAGE_PAT { 1UL << 12 };
    PUBLIC constexpr CONST auto PAGE_GLOBAL { 1UL << 8 };
    PUBLIC constexpr CONST auto PAGE_PS { 1UL << 7 };
    PUBLIC constexpr CONST auto PAGE_DIRTY { 1UL << 6 };
    PUBLIC constexpr CONST auto PAGE_ACCESSED { 1UL << 5 };
    PUBLIC constexpr CONST auto PAGE_PCD { 1UL << 4 };
    PUBLIC constexpr CONST auto PAGE_PWT { 1UL << 3 };
    PUBLIC constexpr CONST auto PAGE_PRESENT { 1UL << 0 };
    PUBLIC constexpr CONST auto PAGE_RW_W { 1 << 1 };
    PUBLIC constexpr CONST auto PAGE_US_S { 1UL << 0 };
    PUBLIC constexpr CONST auto PAGE_US_U { 1 << 2 };
    PUBLIC constexpr CONST auto PAGE_RW_R { 1UL << 0 };
    PUBLIC constexpr CONST auto PAGE_KERNEL_GDT { PAGE_RW_W | PAGE_PRESENT };
    PUBLIC constexpr CONST auto PAGE_KERNEL_DIR { PAGE_RW_W | PAGE_PRESENT };
    PUBLIC constexpr CONST auto PAGE_KERNEL_PAGE { PAGE_PS | PAGE_RW_R | PAGE_PRESENT };
    PUBLIC constexpr CONST auto PAGE_USER_GDT { PAGE_US_U | PAGE_RW_W | PAGE_PRESENT };
    PUBLIC constexpr CONST auto PAGE_USER_DIR { PAGE_US_U | PAGE_RW_W | PAGE_PRESENT };
    PUBLIC constexpr CONST auto PAGE_USER_PAGE { PAGE_PS | PAGE_US_U | PAGE_RW_W | PAGE_PRESENT };
    PUBLIC constexpr CONST auto PDPT_PAGE_1G { 1 << 7 };
    PUBLIC constexpr CONST auto PD_PAGE_2M { 1 << 7 };
    PUBLIC constexpr CONST auto PT_PAGE_PAT { 1 << 7 };
    PUBLIC constexpr CONST auto PT_PAGE_1G { 1 << 8 };
    PUBLIC constexpr CONST auto PE_P { 1 };
    PUBLIC constexpr CONST auto PE_RW { 1 << 1 };
    PUBLIC constexpr CONST auto PE_US { 1 << 2 };
    PUBLIC constexpr CONST auto PE_PWT { 1 << 3 };
    PUBLIC constexpr CONST auto PE_PCD { 1 << 4 };
    PUBLIC constexpr CONST auto PE_ACCESSED { 1 << 5 };
    PUBLIC constexpr CONST auto PE_DIR { 1 << 6 };
    PUBLIC constexpr CONST auto PDPTE_1G { 1 << 7 };
    PUBLIC constexpr CONST auto PDE_2M { 1 << 7 };
    PUBLIC constexpr CONST auto PTE_PAT { 1 << 7 };
    PUBLIC constexpr CONST auto PTE_GLOBAL { 1 << 8 };
    /**
     * @brief 将物理地址转换为线性地址
     */
    PUBLIC inline constexpr auto physical_to_virtual( auto address )->Lib::Types::Ptr< VOID > {
        return reinterpret_cast< Lib::Types::Ptr< VOID > >( ( QuantumNEC::Lib::Types::uint64_t )( address ) + KERNEL_BASE_ADDRESS );
    }
    /**
     * @brief 将线性地址转换为物理地址
     */
    PUBLIC inline constexpr auto virtual_to_physical( auto address )->Lib::Types::Ptr< VOID > {
        return reinterpret_cast< Lib::Types::Ptr< VOID > >( ( QuantumNEC::Lib::Types::uint64_t )(address)-KERNEL_BASE_ADDRESS );
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
        using BitMapEntry = Lib::Types::uint8_t;

    public:
        enum class MemoryPageType {
            PAGE_4K,
            PAGE_2M,
            PAGE_1G,
        };

    public:
        explicit PageMemoryManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true );

    public:
        virtual ~PageMemoryManagement( VOID ) noexcept( true ) = default;

    public:
        STATIC auto malloc( IN Lib::Types::size_t size, IN MemoryPageType type ) -> Lib::Types::Ptr< VOID >;
        STATIC auto free( IN Lib::Types::Ptr< VOID > address, IN Lib::Types::size_t size, IN MemoryPageType type ) -> VOID;

    private:
        STATIC auto malloc_2M_page( IN Lib::Types::size_t size ) -> Lib::Types::Ptr< VOID >;
        STATIC auto free_2M_page( IN Lib::Types::Ptr< VOID > address, IN Lib::Types::size_t size ) -> VOID;
        STATIC auto malloc_4K_page( IN Lib::Types::size_t size ) -> Lib::Types::Ptr< VOID >;
        STATIC auto free_4K_page( IN Lib::Types::Ptr< VOID > address, IN Lib::Types::size_t size ) -> VOID;
        STATIC auto malloc_1G_page( IN Lib::Types::size_t size ) -> Lib::Types::Ptr< VOID >;
        STATIC auto free_1G_page( IN Lib::Types::Ptr< VOID > address, IN Lib::Types::size_t size ) -> VOID;

    public:
        inline STATIC Lib::Types::size_t memory_total { };
        inline STATIC Lib::Types::size_t general_memory_total { };

    private:
        inline STATIC BitMapEntry page_descriptor_entry[ MEMORY_PAGE_DESCRIPTOR ] { };
        inline STATIC Lib::Base::BitmapManagement bitmap_ { };
    };
}