#include <Boot/Graphics.hpp>
#include <Boot/Logger.hpp>
#include <Boot/Memory.hpp>
#include <Boot/Utils.hpp>
#include <Boot/Include.hpp>
constexpr CONST auto PAGE_4K { ( 0x1000 ) };
constexpr CONST auto PAGE_4KSHIFT { ( 12 ) };
constexpr CONST auto PAGE_4KMASK { ( 0xfff ) };
constexpr CONST auto PAGE_2M { ( 0x200000 ) };
constexpr CONST auto PAGE_2MSHIFT { ( 21 ) };
constexpr CONST auto PAGE_2MMASK { ( 0x1fffff ) };
constexpr CONST auto PAGE_P { 1ull << 0 };
constexpr CONST auto PAGE_RW { 1ull << 1 };
constexpr CONST auto PAGE_US { 1ull << 2 };
constexpr CONST auto PAGE_WT { 1ull << 3 };
constexpr CONST auto PAGE_CD { 1ull << 4 };
constexpr CONST auto PAGE_AC { 1ull << 5 };
constexpr CONST auto PAGE_DIRTY { 1ull << 6 };      //  仅在PD/PTE中有效
constexpr CONST auto PAGE_PAT { 1ull << 7 };        //  仅在PD/PTE中有效
constexpr CONST auto PAGE_GLOBAL { 1ull << 8 };     //  仅在PD/PTE中有效
constexpr CONST auto PAGE_NX { 1ull << 63 };
namespace QuantumNEC::Boot {
BootServiceMemory::BootServiceMemory( IN MemoryConfig *config ) :
    BootServiceDataManage< MemoryConfig > {
    config
}
{
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the memory service management." );
    logger.Close( );
    displayStep( );
}
auto BootServiceMemory::getMemoryMap( VOID ) -> EFI_STATUS {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    EFI_STATUS Status { EFI_SUCCESS };
    // 设置config
    this->put( ).Buffer = NULL;
    this->put( ).DescriptorSize = 0;
    this->put( ).DescriptorVersion = 0;
    this->put( ).MemoryKey = 0;
    this->put( ).MemoryCount = 0;
    this->put( ).MemorySize = 0;
    // 获取memory map
    while ( gBS->GetMemoryMap( &this->put( ).MemorySize, this->put( ).Buffer, &this->put( ).MemoryKey, &this->put( ).DescriptorSize, &this->put( ).DescriptorVersion ) == EFI_BUFFER_TOO_SMALL ) {
        if ( this->put( ).Buffer ) {
            delete this->put( ).Buffer;
            this->put( ).Buffer = NULL;
        }
        this->put( ).Buffer = new EFI_MEMORY_DESCRIPTOR[ this->put( ).MemorySize ];     // 为memory map分配内存块
        if ( !this->put( ).Buffer ) {
            logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to get Memory Map." );
            logger.LogError( EFI_INVALID_PARAMETER );
            logger.Close( );
            return EFI_INVALID_PARAMETER;
        }
    }
    // 获得内存块数量
    this->put( ).MemoryCount = this->put( ).MemorySize / this->put( ).DescriptorSize;
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Get Memory Map." );
    logger.Close( );
    return Status;
}
auto BootServicePage::map( IN UINT64 pml4TableAddress, IN UINT64 physicsAddress, IN UINT64 virtualAddress, IN UINT64 size, IN UINT64 flags, IN MemoryMode mode ) -> decltype( auto ) {
    EFI_STATUS Status { EFI_SUCCESS };
    UINT64 pageSize { ( mode == MemoryMode::MEMORY_4K ) ? UINT64( SIZE_4KB ) : ( ( mode == MemoryMode::MEMORY_2M ) ? UINT64( SIZE_2MB ) : SIZE_1GB ) };
    physicsAddress = physicsAddress - physicsAddress % pageSize;
    virtualAddress = virtualAddress - virtualAddress % pageSize;
    UINT64 pageNumber { ( !( size % pageSize ) ? size / pageSize : ( size - size % pageSize ) / pageSize + 1 ) };
    this->PageTableProtect( false );     // 消除页保护, 因为要更改页
    UINT64 *pml4Table { reinterpret_cast< UINT64 * >( pml4TableAddress ) };
    UINT64 *pd_pt { }, *pd { }, *pt { };
    for ( UINTN Idx { }; Idx < pageNumber;
          ++Idx, physicsAddress += pageSize, virtualAddress += pageSize ) {
        if ( ~pml4Table[ VIRT_PML4E_IDX( virtualAddress ) ] & PE_P )     // 没有找到pd_pt
        {
            pd_pt = reinterpret_cast< UINT64 * >( AllocatePages( 1 ) );
            ZeroMem( pd_pt, SIZE_4KB );
            pml4Table[ VIRT_PML4E_IDX( virtualAddress ) ] = PE_S_ADDRESS( pd_pt ) | PE_P | PE_RW;
        }
        else     // 如果找到了
        {
            pd_pt = reinterpret_cast< UINT64 * >(
                PE_V_ADDRESS( pml4Table[ VIRT_PML4E_IDX( virtualAddress ) ] ) );
        }
        // 如果模式为MEMMOY_1G那么设置1G的内存页并回转
        if ( mode == MemoryMode::MEMORY_1G ) {
            pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] = PE_S_ADDRESS( physicsAddress ) | flags | PDPTE_1G;
            pd_pt = NULL;
            pd = NULL;
            pt = NULL;
            continue;
        }
        // 如果没有找到pd_pt
        if ( ~pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] & PE_P ) {
            // 那么设置pd为1个页并清零
            pd = reinterpret_cast< UINT64 * >( AllocatePages( 1 ) );
            ZeroMem( pd, SIZE_4KB );
            pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] = PE_S_ADDRESS( pd ) | PE_P | PE_RW;
        }
        else     // 如果找到
        {
            // 设置pd
            pd = (UINT64 *)PE_V_ADDRESS( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] );
        }
        // 如果模式为MEMORY_2M那么设置2M的内存页并回转
        if ( mode == MemoryMode::MEMORY_2M ) {
            pd[ VIRT_PDE_IDX( virtualAddress ) ] = PE_S_ADDRESS( physicsAddress ) | flags | PDE_2M;
            pd_pt = NULL;
            pd = NULL;
            pt = NULL;
            continue;
        }
        // 未找到pt
        if ( ~pd[ VIRT_PDE_IDX( virtualAddress ) ] & PE_P ) {
            // 那么设置pt为1个内存页并清零
            pt = reinterpret_cast< UINT64 * >( AllocatePages( 1 ) );
            ZeroMem( pt, SIZE_4KB );
            pt[ VIRT_PDE_IDX( virtualAddress ) ] = PE_S_ADDRESS( pt ) | PE_P | PE_RW;
        }
        else {
            pt = reinterpret_cast< UINT64 * >(
                PE_V_ADDRESS( pd[ VIRT_PDE_IDX( virtualAddress ) ] ) );
        }
        // 最后的情况，模式为MEMORY_4K
        pt[ VIRT_PTE_IDX( virtualAddress ) ] = PE_S_ADDRESS( physicsAddress ) | flags;
        pd_pt = NULL;
        pd = NULL;
        pt = NULL;
    }

    this->PageTableProtect( TRUE );

    return Status;
}
auto BootServicePage::isPageMapped( IN UINT64 pml4TableAddress, IN UINT64 virtualAddress ) -> decltype( auto ) {
    virtualAddress = virtualAddress - virtualAddress % SIZE_4KB;

    UINT64 *pml4Table { reinterpret_cast< UINT64 * >( pml4TableAddress ) };
    UINT64 *pd_pt { }, *pd { }, *pt { };
    // 未找到pml4 Table
    if ( ~pml4Table[ VIRT_PML4E_IDX( virtualAddress ) ] & PE_P ) {
        return FALSE;
    }
    else {
        pd_pt = reinterpret_cast< UINT64 * >(
            PE_V_ADDRESS( pml4Table[ VIRT_PML4E_IDX( pml4TableAddress ) ] ) );
    }
    // 未找到pd
    if ( ~pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] & PE_P ) {
        return FALSE;
    }
    else {
        if ( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] & PDPTE_1G ) {
            return TRUE;
        }
        pd = (UINT64 *)PE_V_ADDRESS( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] );
    }
    // 未找到pt
    if ( ~pd[ VIRT_PDE_IDX( virtualAddress ) ] & PE_P ) {
        return FALSE;
    }
    else {
        if ( pd[ VIRT_PDPTE_IDX( virtualAddress ) ] & PDE_2M ) {
            return TRUE;
        }
        pt = reinterpret_cast< UINT64 * >(
            PE_V_ADDRESS( pd[ VIRT_PDE_IDX( virtualAddress ) ] ) );
    }
    // 未找到PG
    if ( ~pt[ VIRT_PTE_IDX( virtualAddress ) ] & PE_P ) {
        return FALSE;
    }
    return TRUE;
}
auto BootServicePage::VTPAddress( IN UINT64 pml4TableAddress, IN UINT64 virtualAddress ) -> decltype( auto ) {
    pml4TableAddress &= ~0x7FF;
    virtualAddress &= ~0x7FF;

    UINT64 *pml4Table { reinterpret_cast< UINT64 * >( pml4TableAddress ) };
    UINT64 *pd_pt { }, *pd { }, *pt { };
    // 如果未映射此页面，则返回 FALSE
    if ( !this->isPageMapped( pml4TableAddress, virtualAddress ) )
        return static_cast< UINT64 >( FALSE );
    /* 逐步找到页面 */
    pd_pt = reinterpret_cast< UINT64 * >(
        PE_V_ADDRESS( pml4Table[ VIRT_PML4E_IDX( virtualAddress ) ] ) );
    if ( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] & PDPTE_1G ) {
        return PE_V_ADDRESS( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] );
    }
    pd = reinterpret_cast< UINT64 * >( PE_V_ADDRESS( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] ) );
    if ( pd[ VIRT_PDE_IDX( virtualAddress ) ] & PDE_2M ) {
        return PE_V_ADDRESS( pd[ VIRT_PDE_IDX( virtualAddress ) ] );
    }
    pt = reinterpret_cast< UINT64 * >(
        PE_V_ADDRESS( pd[ VIRT_PDE_IDX( virtualAddress ) ] ) );
    return PE_V_ADDRESS( pt[ VIRT_PTE_IDX( virtualAddress ) ] )
           | VIRT_OFFSET( virtualAddress );
}
auto BootServicePage::pml4TableDump( IN UINT64 *pml4Table ) -> decltype( auto ) {
    if ( !pml4Table )
        return;
    UINT64 virtualAddress { };
    UINT64 *pd_pt { }, *pd { }, *pt { };
    UINT16 ipml4E { }, ipd_ptE { }, ipdE { }, iptE { };
    // PML4 Level
    while ( ipml4E < 512 ) {
        if ( ~*pml4Table & PE_P ) {
            virtualAddress += PDPT_S;
            continue;
        }
        pd_pt = reinterpret_cast< UINT64 * >( PE_V_ADDRESS( *pml4Table ) );
        // PDPT Level
        while ( ipd_ptE < 512 ) {
            if ( ~*pd_pt & PE_P || *pd_pt & PDPTE_1G ) {
                virtualAddress += PD_S;
                continue;
            }
            pd = reinterpret_cast< UINT64 * >( PE_V_ADDRESS( *pd_pt ) );
            // PD Level
            while ( ipdE < 512 ) {
                if ( ~*pd & PE_P || *pd & PDE_2M ) {
                    virtualAddress += PT_S;
                    continue;
                }
                pt = reinterpret_cast< UINT64 * >( PE_V_ADDRESS( *pd ) );
                // PT level
                while ( iptE < 512 ) {
                    if ( ~*pt & PE_P ) {
                        virtualAddress += PG_S;
                        continue;
                    }
                    virtualAddress += PG_S;
                    ++iptE, ++pt;
                }
                ++ipdE, ++pd;
            }
            ++ipd_ptE, ++pd_pt;
        }
        ++ipml4E, ++pml4Table;
    }
}
auto BootServicePage::updateCr3( VOID ) -> UINTN {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "Update CR3 register." );

    UINTN ret { AsmWriteCr3( reinterpret_cast< UINT64 >( this->pageTable ) ) };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Update CR3 register OK." );
    logger.Close( );
    return ret;
    
}
BootServicePage::BootServicePage( IN PageConfig *memoryPages ) :
    BootServiceDataManage< PageConfig > {
    memoryPages
}
{
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS,
                   "Initialize the memory pages service management." );
    logger.Close( );
    displayStep( );
}
//  支持5级分页就是pml5 不然是pml4

auto BootServicePage::setPageTable( ) -> EFI_STATUS {
    STATIC bool is5LevelPagingSupport = FALSE;
    STATIC UINT64 kernelPageMap { };
    STATIC UINT64 kernelPageDirectoryCount { };
    // 设置页表
    UINT8 physicalAddressBits { };
    UINT32 pml5EntryCount { 1 };
    UINT32 pml4EntryCount { 1 };
    UINT32 pdpEntryCount { 1 };
    UINT32 kernelPageTableSize { };
    UINT32 tmp { };
    UINT32 eax { }, ecx { };
    _cpuid( 0x7, tmp, tmp, ecx, tmp );     // 查询各个寄存器
    is5LevelPagingSupport = ( ecx & ( 1ull << 16 ) != 0 );
    _cpuid( 0x80000000, eax, tmp, tmp, tmp );
    if ( eax >= 0x80000008 ) {
        _cpuid( 0x80000008, eax, tmp, tmp, tmp );
        physicalAddressBits = static_cast< UINT8 >( eax );
    }
    else {
        physicalAddressBits = 36;
    }
    if ( physicalAddressBits > 48 ) {
        if ( is5LevelPagingSupport ) {
            pml5EntryCount = 1ull << ( physicalAddressBits - 48 );
        }
        physicalAddressBits = 48;
    }
    if ( physicalAddressBits > 39 ) {
        pml4EntryCount = 1ull << ( physicalAddressBits - 39 );
        physicalAddressBits = 39;
    }
    pdpEntryCount = 1ull << ( physicalAddressBits - 30 );
    kernelPageTableSize = ( ( pdpEntryCount + 1 ) * pml4EntryCount + 1 ) * pml5EntryCount + ( is5LevelPagingSupport ? 1 : 0 );
    kernelPageDirectoryCount = ( (pdpEntryCount)*pml4EntryCount ) * pml5EntryCount * 512;
    VOID *kernelPageTable = AllocatePages( kernelPageTableSize );
    UINT64 pageTableAddress { reinterpret_cast< UINT64 >( kernelPageTable ) };
    this->pageTable = pageTableAddress;
    UINT64 *pml5Entry { };
    UINT64 *pml4Entry { };
    UINT64 *pdpEntry { };
    UINT64 *pdEntry { };
    UINT64 pageAddress { };
    if ( is5LevelPagingSupport ) {
        pml5Entry = (UINT64 *)pageTableAddress;
        pageTableAddress += PAGE_4K;
    }
    for ( UINT64 pml5Index { }; pml5Index < (UINT64)( pml5EntryCount ); ++pml5Index ) {
        pml4Entry = (UINT64 *)pageTableAddress;
        pageTableAddress += PAGE_4K;

        if ( is5LevelPagingSupport ) {
            *pml5Entry++ = (UINT64)pml4Entry | PAGE_P | PAGE_RW;
        }

        for ( UINT64 pml4Index { }; pml4Index < (UINT64)( pml5EntryCount == 1 ? pml4EntryCount : 512 ); ++pml4Index ) {
            pdpEntry = (UINT64 *)pageTableAddress;
            pageTableAddress += PAGE_4K;

            *pml4Entry++ = (UINT64)pdpEntry | PAGE_P | PAGE_RW;

            for ( UINT64 pdpIndex { }; pdpIndex < (UINT64)( pml4EntryCount == 1 ? pdpEntryCount : 512 ); ++pdpIndex ) {
                pdEntry = (UINT64 *)pageTableAddress;
                pageTableAddress += PAGE_4K;

                *pdpEntry++ = (UINT64)pdEntry | PAGE_P | PAGE_RW;

                for ( UINT64 pdIndex { }; pdIndex < 512; ++pdIndex ) {
                    *pdEntry++ = (UINT64)pageAddress | PAGE_P | PAGE_RW | ( 1ull << 7 );
                    pageAddress += PAGE_2M;
                }
            }
        }
    }
    this->updateCr3( );
    return EFI_SUCCESS;
}
}     // namespace QuantumNEC::Boot