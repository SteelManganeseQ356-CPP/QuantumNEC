#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Kernel::Memory {
    PUBLIC constexpr CONST auto R_IDX { 511ULL };
    PUBLIC typedef struct
    {
        Lib::Types::Ptr< Lib::Types::uint64_t > pml4t;
    } pml4t_t;
    PUBLIC inline constexpr auto mk_mpl4t( auto addr, auto attr ) {
        return ( (unsigned long)( addr ) | (unsigned long)( attr ) );
    }
    PUBLIC template < typename _Tx, typename _Ty >
        requires requires( _Tx x, _Ty y ) {
            { *x = y };
        }
    inline constexpr auto set_mpl4t( _Tx mpl4tptr, _Ty mpl4tval ) {
        *mpl4tptr = mpl4tval;
    }
    typedef struct
    {
        Lib::Types::uint64_t pdpt;
    } pdpt_t;
    PUBLIC inline constexpr auto mk_pdpt( auto addr, auto attr ) {
        return ( (unsigned long)( addr ) | (unsigned long)( attr ) );
    }
    PUBLIC template < typename _Tx, typename _Ty >
        requires requires( _Tx x, _Ty y ) {
            { *x = y };
        }
    inline constexpr auto set_pdpt( _Tx pdptptr, _Ty pdptval ) {
        *pdptptr = pdptval;
    }

    PUBLIC typedef struct
    {
        Lib::Types::uint64_t pdt;
    } pdt_t;
    PUBLIC inline constexpr auto mk_pdt( auto addr, auto attr ) {
        return ( (unsigned long)( addr ) | (unsigned long)( attr ) );
    }
    PUBLIC template < typename _Tx, typename _Ty >
        requires requires( _Tx x, _Ty y ) {
            { *x = y };
        }
    inline constexpr auto set_pdt( _Tx pdtptr, _Ty pdtval ) {
        *pdtptr = pdtval;
    }
    PUBLIC typedef struct
    {
        Lib::Types::uint64_t pt;
    } pt_t;
    PUBLIC inline constexpr auto mk_pt( auto addr, auto attr ) {
        return (unsigned long)( addr ) | (unsigned long)( attr );
    }
    PUBLIC template < typename _Tx, typename _Ty >
        requires requires( _Tx x, _Ty y ) {
            { *x = y };
        }
    inline constexpr auto set_pt( _Tx ptptr, _Ty ptval ) {
        *ptptr = ptval;
    }
    PUBLIC class MemoryMapManagement
    {
    private:
        /**
         * @brief 映射类型
         */
        enum class MapType {
            PML4 = 4,
            PDPT = 3,
            PD = 2,
            PT = 1,
            PG = 0,
        };
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
         * @param physicsAddress 要映射的物理地址
         * @param virtualAddress 将物理地址映射的指定虚拟地址
         * @param size 要映射的内存页大小
         * @param flags 映射标志
         * @param mode 映射页的模式 (4K/2M/1G)
         */
        auto map( IN Lib::Types::uint64_t physicsAddress, IN Lib::Types::uint64_t virtualAddress, IN Lib::Types::size_t size, IN Lib::Types::uint16_t flags, IN MapMode mode ) -> VOID;

    private:
        /**
         * @brief 获取入口
         * @param level 获取的东西类型
         * @return 入口地址
         */
        auto getEntry( IN Lib::Types::Ptr< VOID > address, IN Lib::Types::int32_t level ) -> Lib::Types::Ptr< Lib::Types::uint64_t >;
        /**
         * @brief 检查虚拟地址是否为规范格式vrt地址，如果无效，请进行调整
         * @param virtualAddress 要检查的地址
         * @retval BOOL 状态
         */
        auto checkAndAdjust( IN Lib::Types::Ptr< Lib::Types::uint64_t > virtualAddress ) -> Lib::Types::BOOL;

        /**
         * @brief 将虚拟地址转换为物理地址
         * @param virtualAddress 要转换的虚拟地址
         * @param size 要转换的页数
         * @param flags 转换标志
         */
        auto physicalAuto( IN Lib::Types::uint64_t virtualAddress, IN Lib::Types::size_t size, Lib::Types::uint16_t flags ) -> Lib::Types::Ptr< VOID >;
        /**
         * @brief 辅助map
         */
        auto mapWalk( IN Lib::Types::uint64_t physicsAddress, IN Lib::Types::uint64_t virtualAddress, IN Lib::Types::uint16_t flags, IN Lib::Types::Ptr< VOID > table, IN Lib::Types::int32_t level, IN MapMode mode ) -> VOID;
    };
}
