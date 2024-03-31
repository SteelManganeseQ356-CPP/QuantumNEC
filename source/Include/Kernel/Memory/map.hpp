#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>

PUBLIC namespace QuantumNEC::Kernel::Memory {
    constexpr CONST auto PML_IDX { 512 };
    constexpr CONST auto PML_SIZE { PML_IDX * 2 };
    constexpr CONST auto PT_SIZE { 0x1000 };
    constexpr CONST auto MEMORY_SIZE_4K { 65536 };
    constexpr CONST auto MEMORY_SIZE_2M { 128 };
    constexpr CONST auto MEMORY_SIZE_1G { 1 };
    PUBLIC typedef struct
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
    } pml_t;
    PUBLIC typedef struct
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
    } pdpt_t;
    PUBLIC typedef struct
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
    } pdt_t;
    PUBLIC typedef struct
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
    } pt_t;
    PUBLIC class MemoryMapManagement
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
        explicit( true ) MemoryMapManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true );

    public:
        virtual ~MemoryMapManagement( VOID ) noexcept( true ) = default;

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
        STATIC auto remap( IN Lib::Types::uint64_t virtual_address, IN Lib::Types::size_t size, IN Lib::Types::Ptr< Lib::Types::uint64_t > pml = page_memory_table.pml ) -> VOID;

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

    private:
        STATIC inline pml_t page_memory_table { };
    };
}
