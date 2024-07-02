#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Kernel {
    PUBLIC constexpr CONST auto PML_IDX { 512 };
    PUBLIC constexpr CONST auto PML_SIZE { PML_IDX * 2 };
    PUBLIC constexpr CONST auto PT_SIZE { 0x1000 };

    PUBLIC enum class MemoryPageType : Lib::Types::uint64_t {
        PAGE_4K = 65536,
        PAGE_2M = 128,
        PAGE_1G = 1,
    };
    PUBLIC struct pml_t
    {
        Lib::Types::Ptr< Lib::Types::uint64_t > pml;
        auto make_mplt( IN Lib::Types::Ptr< Lib::Types::uint64_t > address, IN Lib::Types::uint64_t attribute ) {
            *this->pml = ( reinterpret_cast< Lib::Types::uint64_t >( address ) | attribute );
            return pml;
        }
        auto set_mplt( IN Lib::Types::Ptr< Lib::Types::uint64_t > mplval ) {
            this->pml = mplval;
            return;
        }
    };
    PUBLIC struct pdpt_t
    {
        Lib::Types::Ptr< Lib::Types::uint64_t > pdpt;
        auto make_pdpt( IN Lib::Types::Ptr< Lib::Types::uint64_t > address, IN Lib::Types::uint64_t attribute ) {
            *this->pdpt = ( reinterpret_cast< Lib::Types::uint64_t >( address ) | attribute );
            return this->pdpt;
        }
        auto set_pdpt( IN Lib::Types::Ptr< Lib::Types::uint64_t > pdptval ) {
            this->pdpt = pdptval;
            return;
        }
    };
    PUBLIC struct pdt_t
    {
        Lib::Types::Ptr< Lib::Types::uint64_t > pdt;
        auto make_pdt( IN Lib::Types::Ptr< Lib::Types::uint64_t > address, IN Lib::Types::uint64_t attribute ) {
            *this->pdt = ( reinterpret_cast< Lib::Types::uint64_t >( address ) | attribute );
            return this->pdt;
        }
        auto set_pdt( IN Lib::Types::Ptr< Lib::Types::uint64_t > pdtval ) {
            this->pdt = pdtval;
            return;
        }
    };
    PUBLIC struct pt_t
    {
        Lib::Types::Ptr< Lib::Types::uint64_t > pt;
        auto make_pt( IN Lib::Types::Ptr< Lib::Types::uint64_t > address, IN Lib::Types::uint64_t attribute ) {
            *this->pt = ( reinterpret_cast< Lib::Types::uint64_t >( address ) | attribute );
            return this->pt;
        }
        auto set_pt( IN Lib::Types::Ptr< Lib::Types::uint64_t > ptval ) {
            this->pt = ptval;
            return;
        }
    };

    PUBLIC class Table
    {
    private:
        Lib::Types::uint64_t present : 1;
        Lib::Types::uint64_t read_write : 1;
        Lib::Types::uint64_t user_supervisor : 1;
        Lib::Types::uint64_t write_through : 1;
        Lib::Types::uint64_t cache_disable : 1;
        Lib::Types::uint64_t accessed : 1;
        Lib::Types::uint64_t dirty : 1;
        Lib::Types::uint64_t page_attribute_table_page_size : 1;
        Lib::Types::uint64_t global : 1;
        Lib::Types::uint64_t available : 3;
        Lib::Types::uint64_t address : 40;
        Lib::Types::uint64_t reserved : 7;
        Lib::Types::uint64_t protection_key : 4;
        Lib::Types::uint64_t execute_disable : 1;
        // Lib::Types::uint64_t table;

    public:
        constexpr auto flags_p( VOID ) {
            return 1ull << 0;
        }
        constexpr auto flags_rw( VOID ) {
            return 1ull << 1;
        }
        constexpr auto flags_us( VOID ) {
            return 1ull << 2;
        }
        constexpr auto flags_pwt( VOID ) {
            return 1ull << 3;
        }
        constexpr auto flags_pcd( VOID ) {
            return 1ull << 4;
        }
        constexpr auto flags_a( VOID ) {
            return 1ull << 5;
        }
        constexpr auto flags_d( VOID ) {
            return 1ull << 6;
        }
        constexpr auto flags_pat_ps( VOID ) {
            return 1ull << 7;
        }
        constexpr auto flags_g( VOID ) {
            return 1ull << 9;
        }
        constexpr auto flags_avl( VOID ) {
            return 1ull << 8;
        }
        constexpr auto flags_xd( VOID ) {
            return 1ull << 63;
        }
        constexpr auto set_p( IN Lib::Types::uint64_t bit ) {
            this->present = bit;
        }
        constexpr auto set_rw( IN Lib::Types::uint64_t bit ) {
            this->read_write = bit;
        }
        constexpr auto set_us( IN Lib::Types::uint64_t bit ) {
            this->user_supervisor = bit;
        }
        constexpr auto set_pwt( IN Lib::Types::uint64_t bit ) {
            this->write_through = bit;
        }
        constexpr auto set_pcd( IN Lib::Types::uint64_t bit ) {
            this->cache_disable = bit;
        }
        constexpr auto set_a( IN Lib::Types::uint64_t bit ) {
            this->accessed = bit;
        }
        constexpr auto set_d( IN Lib::Types::uint64_t bit ) {
            this->dirty = bit;
        }
        constexpr auto set_ps_pat( IN Lib::Types::uint64_t bit ) {
            this->page_attribute_table_page_size = bit;
        }

        constexpr auto set_g( IN Lib::Types::uint64_t bit ) {
            this->global = bit;
        }
        constexpr auto set_avl( IN Lib::Types::uint64_t bit ) {
            this->available = bit;
        }

        constexpr auto set_pk( IN Lib::Types::uint64_t bit ) {
            this->protection_key = bit;
        }
        constexpr auto set_xd( IN Lib::Types::uint64_t bit ) {
            this->execute_disable = bit;
        }
        auto set_address( IN Lib::Types::uint64_t bit ) {
            *reinterpret_cast< Lib::Types::uint64_t * >( this ) |= bit;
        }

    public:
        constexpr auto set_table( IN Lib::Types::Ptr< VOID > address, IN Lib::Types::uint64_t flags ) -> VOID;
    };

    PUBLIC class PageDirectoryTable     // 4096*513
    {
    public:
        explicit PageDirectoryTable( VOID ) noexcept = default;

    public:
        /**
         * @brief 设置512个页表到512个pd
         * @param flags 设置位
         * @param type 内存页内存
         */
        auto make( IN Lib::Types::uint64_t flags, IN MemoryPageType type ) noexcept -> VOID;

    private:
        Table page_directory_table[ 512 ];     // 4096
    };
    PUBLIC class PageDirectoryPointerTable     // 4096*513
    {
    public:
        explicit PageDirectoryPointerTable( VOID ) noexcept = default;

    public:
        /**
         * @brief 设置512个pd到512个pdp
         * @param flags 设置位
         * @param type 内存页内存
         */
        auto make( IN Lib::Types::uint64_t flags, IN MemoryPageType type ) noexcept -> VOID;
        auto set_count( IN Lib::Types::uint64_t count ) {
            this->pdp_entry_count = count;
        }

    private:
        Table page_directory_pointer_table[ 512 ];          // 4K
        PageDirectoryTable page_dircetory_table[ 512 ];     // 4096*512
        Lib::Types::uint64_t pdp_entry_count;
    };

    PUBLIC class PageMapLevel4Table     // 4096*513*513
    {
    public:
        explicit PageMapLevel4Table( VOID ) noexcept;

    public:
        auto make( IN Lib::Types::uint64_t flags, IN MemoryPageType type ) noexcept -> VOID;

    public:
        constexpr auto size( VOID ) {
            return ( sizeof *this ) * pml4e_entry_count;
        }

    private:
        Table page_map_level4_table[ 512 ];
        PageDirectoryPointerTable page_directory_pointer_table[ 512 ];     // 4096*513 *513
        Lib::Types::uint64_t pdp_entry_count, pml4e_entry_count;
    };

    PUBLIC class MemoryMap
    {
    private:
        /**
         * @brief 映射类型
         */
        enum class MapLevel {
            PML = 4,
            PDPT = 3,
            PD = 2,
            PT = 1,
            PG = 0,
        };

    public:
        /**
         * @brief 内存页模式
         */
        enum class MapMode {
            MEMORY_MAP_4K = 0x1,      // PTE
            MEMORY_MAP_2M = 0x2,      // PDE
            MEMORY_MAP_1G = 0x3,      // PDPTE
            MEMORY_MAP_END = 0x4,     // End
        };

    public:
        explicit MemoryMap( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept;

    public:
        virtual ~MemoryMap( VOID ) noexcept = default;

    public:
        /**
         * @brief 映射页
         * @param physics_address 要映射的物理地址
         * @param virtual_address 将物理地址映射的指定虚拟地址
         * @param size 要映射的内存页大小
         * @param flags 映射标志
         * @param mode 映射页的模式 (4K/2M/1G)
         */
        STATIC auto map( IN Lib::Types::uint64_t physics_address, IN Lib::Types::uint64_t virtual_address, IN Lib::Types::size_t size, IN Lib::Types::uint16_t flags, IN MapMode mode, IN Lib::Types::Ptr< Lib::Types::uint64_t > pml = page_memory_table.pml ) -> VOID;
        /**
         * @brief 取消映射页
         * @param virtual_address 将取消映射的指定虚拟地址
         * @param size 要取消映射的内存页大小
         */
        STATIC auto unmap( IN Lib::Types::uint64_t virtual_address, IN Lib::Types::size_t size, IN Lib::Types::Ptr< Lib::Types::uint64_t > pml = page_memory_table.pml ) -> VOID;

    private:
        /**
         * @brief 获取入口
         * @param level 获取的东西类型
         * @return 入口地址
         */
        auto get_table_entry( IN Lib::Types::Ptr< VOID > address, IN MapLevel level ) -> Lib::Types::Ptr< Lib::Types::uint64_t >;
        /**
         * @brief 检查虚拟地址是否为规范格式vrt地址，如果无效，请进行调整
         * @param virtual_address 要检查的地址
         * @retval BOOL 状态
         */
        auto check( IN Lib::Types::Ptr< Lib::Types::uint64_t > virtual_address ) -> Lib::Types::BOOL;

    public:
        /**
         * @brief 控制页保护开关
         * @param flags 如果为true那么开启页保护，如果为false那么关闭页保护
         */
        STATIC auto page_table_protect( IN Lib::Types::BOOL flags ) -> VOID;
        /**
         * @brief 制作页根目录并给到页表中
         * @return 制作的页目录地址
         */
        STATIC auto make_page_directory_table( VOID ) -> Lib::Types::Ptr< Lib::Types::uint64_t >;
        /**
         * @brief 激活页表
         * @param page_directory 页根目录地址
         */
        STATIC auto activate_page_directory_table( IN Lib::Types::Ptr< VOID > page_directory_table_address ) -> VOID;
        /**
         * @brief 获取当前页表
         */
        STATIC auto get_current_page_tabel( VOID ) -> Lib::Types::Ptr< Lib::Types::uint64_t >;
        /**
         * @brief 获取当前页表根目录
         * @param index 当前页表根目录序号
         */
        STATIC auto get_page_directory( IN pml_t pml, IN Lib::Types::uint32_t index ) -> Lib::Types::Ptr< Lib::Types::uint64_t >;
        /**
         * @brief 获取规范地址掩码
         */
        STATIC auto get_canonical_address_mask( VOID ) -> Lib::Types::uint64_t;

    private:
        STATIC inline pml_t page_memory_table { };
    };
}
