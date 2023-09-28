#include <Kernel/Memory/map.hpp>
#include <Kernel/Memory/paging.hpp>
#include <Utils/asm.hpp>
#include <Lib/STL/string>
#include <Lib/IO/Stream/iostream>
#include <Arch/Arch.hpp>
PUBLIC namespace {
    using namespace QuantumNEC::Lib::Types;
    using namespace QuantumNEC::Lib;
    using namespace QuantumNEC;
    Kernel::Memory::pml4t_t pml4Table { };
    constexpr CONST auto SIZE_4K { 0x00001000U };
    constexpr CONST auto SIZE_2M { 0x00200000U };
    constexpr CONST auto SIZE_1G { 0x40000000U };
    /**
     * @brief 值
     */
    constexpr auto PE_V_FLAGS( auto Entry )->UINT64 {
        return ( (UINT64)Entry ) & 0x7FF;
    }
    constexpr auto PE_V_ADDRESS( auto Entry )->UINT64 {
        return ( (UINT64)Entry ) & ~0x7FF;
    }
    /**
     * @brief 设置
     */
    constexpr auto PE_S_FLAGS( auto Flags )->UINT64 {
        return (UINT64)( Flags & 0x7FF );
    }
    constexpr auto PE_S_ADDRESS( auto Address )->UINT64 {
        return (UINT64)( ( (UINT64)Address & ~0x7FF ) );
    }
    /**
     *  @brief 通过虚拟地址查找条目或其他条目
     */
    constexpr auto VIRT_PML4E_IDX( auto Address )->UINT64 {
        return ( ( (UINT64)Address >> 39 ) & 0x1FF );
    }
    constexpr auto VIRT_PDPTE_IDX( auto Address )->UINT64 {
        return ( ( (UINT64)Address >> 30 ) & 0x1FF );
    }
    constexpr auto VIRT_PDE_IDX( auto Address )->UINT64 {
        return ( ( (UINT64)Address >> 21 ) & 0x1FF );
    }
    constexpr auto VIRT_PTE_IDX( auto Address )->UINT64 {
        return ( ( (UINT64)Address >> 12 ) & 0x1FF );
    }
    /**
     *  @brief 页内偏移
     */
    constexpr auto VIRT_OFFSET( auto Address )->UINT64 {
        return ( (UINT64)Address & 0xFFF );
    }
    constexpr auto VirtualTheAddress( auto pml4, auto pdpt, auto pd, auto pt )->Lib::Types::uint64_t {
        return ( (Lib::Types::uint64_t)pml4 > 255 ? 0xFFFFULL : 0 ) << 48
               | ( (Lib::Types::uint64_t)pml4 & 0x1FF ) << 39
               | ( (Lib::Types::uint64_t)pdpt & 0x1FF ) << 30
               | ( (Lib::Types::uint64_t)pd & 0x1FF ) << 21
               | ( (Lib::Types::uint64_t)pt & 0x1FF ) << 12;
    }
    constexpr auto PML4E_IDX( auto address ) {
        return ( (Lib::Types::uint64_t)address >> 39 ) & 0x1FF;
    }
    constexpr auto PDPTE_IDX( auto address ) {
        return ( (Lib::Types::uint64_t)address >> 30 ) & 0x1FF;
    }
    constexpr auto PDE_IDX( auto address ) {
        return ( (Lib::Types::uint64_t)address >> 21 ) & 0x1FF;
    }
    constexpr auto PTE_IDX( auto address ) {
        return ( (Lib::Types::uint64_t)address >> 12 ) & 0x1FF;
    }
    constexpr CONST auto PE_P { 1 };
    constexpr CONST auto PE_RW { 1 << 1 };
    constexpr CONST auto PE_US { 1 << 2 };
    constexpr CONST auto PE_PWT { 1 << 3 };
    constexpr CONST auto PE_PCD { 1 << 4 };
    constexpr CONST auto PE_ACCESSED { 1 << 5 };
    constexpr CONST auto PE_DIR { 1 << 6 };
    constexpr CONST auto PDPTE_1G { 1 << 7 };
    constexpr CONST auto PDE_2M { 1 << 7 };
    constexpr CONST auto PTE_PAT { 1 << 7 };
    constexpr CONST auto PTE_GLOBAL { 1 << 8 };
}
PUBLIC namespace QuantumNEC::Kernel::Memory {
    auto MemoryMapManagement::getEntry( IN Lib::Types::Ptr< VOID > address, IN Lib::Types::int32_t level )->Lib::Types::Ptr< Lib::Types::uint64_t > {
        if ( level == static_cast< Lib::Types::int32_t >( MapType::PML4 ) ) {
            return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( VirtualTheAddress( R_IDX, R_IDX, R_IDX, R_IDX ) );
        }
        else if ( level == static_cast< Lib::Types::int32_t >( MapType::PDPT ) ) {
            return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( VirtualTheAddress( R_IDX, R_IDX, R_IDX, PML4E_IDX( address ) ) );
        }
        else if ( level == static_cast< Lib::Types::int32_t >( MapType::PD ) ) {
            return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( VirtualTheAddress( R_IDX, R_IDX, PML4E_IDX( address ), PDPTE_IDX( address ) ) );
        }
        else if ( level == static_cast< Lib::Types::int32_t >( MapType::PT ) ) {
            return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( VirtualTheAddress( R_IDX, PML4E_IDX( address ), PDPTE_IDX( address ), PDE_IDX( address ) ) );
        }
        return NULL;
    }
    MemoryMapManagement::MemoryMapManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) {
        auto pageTableProtect = [ &, this ]( IN bool flags ) -> VOID {
            if ( !flags ) {
                Utils::writeCr0( Utils::readCr0( ) & ~0x10000 );
                Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ] << "Disable the page protection." << Lib::IO::endl;
            }
            else {
                Utils::writeCr0( Utils::readCr0( ) | 0x10000 );
                Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ] << "Enable the page protection." << Lib::IO::endl;
            }
        };
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Start mapping the page table." << Lib::IO::endl;
        // 消除页保护
        pageTableProtect( FALSE );
        // 获取在启动时服务设置的页表
        pml4Table.pml4t = reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( Utils::readCr3( ) );
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::INFO ] << "Get a pages table from cr3, address of page table -> " << pml4Table.pml4t << "." << Lib::IO::endl;
        // 设置页表
        pml4Table.pml4t[ R_IDX ] = PE_S_ADDRESS( pml4Table.pml4t ) | PE_RW | PE_P;
        // 映射页表
        // 映射至KERNEL_FB所代表的地址，也就是显存
        this->map( _config->GraphicsData.FrameBufferBase, KERNEL_FB, divRoundUp( _config->GraphicsData.FrameBufferSize, PAGE_SIZE ), PE_RW | PE_P | PTE_GLOBAL, MapMode::MEMORY_MAP_2M );
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::INFO ].printk( Lib::IO::DisplayColor::WHITE, Lib::IO::DisplayColor::BLACK, "Mapping the graphics memory from physical address : [%#018lx] to virtual address : [%#018lx].\n\r", _config->GraphicsData.FrameBufferBase, KERNEL_FB );
        // 开启页保护
        pageTableProtect( TRUE );
        pml4Table.pml4t = this->getEntry( NULL, static_cast< Lib::Types::int32_t >( MapType::PML4 ) );
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Mapping the pages table is OK." << Lib::IO::endl;
    }
    auto MemoryMapManagement::map( IN Lib::Types::uint64_t physicsAddress, IN Lib::Types::uint64_t virtualAddress, IN Lib::Types::size_t size, IN Lib::Types::uint16_t flags, IN MapMode mode )->VOID {
        auto checkMode = [ &, this ] -> Lib::Types::uint32_t {
            return mode == MapMode::MEMORY_MAP_2M ? SIZE_2M : ( mode == MapMode::MEMORY_MAP_1G ? SIZE_1G : SIZE_4K );
        };
        auto checkFlags = [ &, this ] {
            return mode == MapMode::MEMORY_MAP_2M ? PDE_2M : ( mode == MapMode::MEMORY_MAP_1G ? PDPTE_1G : 0 );
        };
        Lib::Types::uint64_t pageSize { checkMode( ) };

        while ( size-- ) {
            mapWalk( physicsAddress, virtualAddress, flags | checkFlags( ), pml4Table.pml4t, static_cast< Lib::Types::int32_t >( MapType::PML4 ), mode );
            physicsAddress += pageSize;
            virtualAddress += pageSize;
        }

        Utils::invlpg( reinterpret_cast< Lib::Types::Ptr< VOID > >( virtualAddress ) );

        return;
    }
    auto MemoryMapManagement::mapWalk( IN Lib::Types::uint64_t physicsAddress, IN Lib::Types::uint64_t virtualAddress, IN Lib::Types::uint16_t flags, IN Lib::Types::Ptr< VOID > table, IN Lib::Types::int32_t level, IN MapMode mode )->VOID {
        Lib::Types::uint64_t index { ( virtualAddress >> ( ( level - 1 ) * 9 + 12 ) ) & 0x1FF };
        if ( static_cast< Lib::Types::int32_t >( level ) == static_cast< Lib::Types::int32_t >( mode ) ) {
            reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( table )[ index ] = PE_S_ADDRESS( physicsAddress ) | PE_S_FLAGS( flags );
            return;
        }
        else if ( !( reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( table )[ index ] & PE_P ) ) {
            Lib::Types::Ptr< VOID > New { PageMemoryManagement::malloc( 1 ) };
            reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( table )[ index ] = PE_S_ADDRESS( New ) | PE_RW | PE_P;
            New = this->getEntry( reinterpret_cast< Lib::Types::Ptr< VOID > >( virtualAddress ), static_cast< Lib::Types::int32_t >( level ) - 1 );

            Lib::STL::memset( New, 0, PAGE_SIZE );
        }
        table = this->getEntry( reinterpret_cast< Lib::Types::Ptr< VOID > >( virtualAddress ), level - 1 );

        this->mapWalk( physicsAddress, virtualAddress, flags, table, level - 1, mode );
    }
    auto MemoryMapManagement::checkAndAdjust( IN Lib::Types::Ptr< Lib::Types::uint64_t > virtualAddress )->Lib::Types::BOOL {
        if ( ( *virtualAddress >> 47 ) & 1 ) {
            if ( *virtualAddress >> 48 == 0xFFFF ) {
                return TRUE;
            }
            *virtualAddress |= 0xFFFFULL << 48;
            return FALSE;
        }
        return TRUE;
    }
    auto MemoryMapManagement::physicalAuto( IN Lib::Types::uint64_t virtualAddress, IN Lib::Types::size_t size, Lib::Types::uint16_t flags )->Lib::Types::Ptr< VOID > {
        if ( !this->checkAndAdjust( &virtualAddress ) ) {
            return NULL;
        }
        Lib::Types::Ptr< VOID > page { PageMemoryManagement::malloc( size ) };
        MemoryMapManagement::map( reinterpret_cast< Lib::Types::uint64_t >( page ), virtualAddress, size, flags, MapMode::MEMORY_MAP_4K );
        return reinterpret_cast< Lib::Types::Ptr< VOID > >( virtualAddress );
    }
}