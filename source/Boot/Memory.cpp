#include <Boot/Memory.hpp>
#include <Boot/Include.hpp>
#include <Boot/Graphics.hpp>
#include <Boot/Logger.hpp>
#include <Boot/Utils.hpp>
namespace QuantumNEC::Boot {
BootServiceMemory::BootServiceMemory( IN MemoryConfig *config ) :
    BootServiceDataManager< MemoryConfig > {
        config
    } {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the memory service management." );
    logger.Close( );
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
    Status = displayStep( );
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
            pd_pt = reinterpret_cast< UINT64 * >( PE_V_ADDRESS( pml4Table[ VIRT_PML4E_IDX( virtualAddress ) ] ) );
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
    // 如果未映射此页面，则返回NULL
    if ( !this->isPageMapped( pml4TableAddress, virtualAddress ) ) {
        return (UINT64)( NULL );
    }
    /* 逐步找到页面 */
    pd_pt = reinterpret_cast< UINT64 * >( PE_V_ADDRESS( pml4Table[ VIRT_PML4E_IDX( virtualAddress ) ] ) );
    if ( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] & PDPTE_1G ) {
        return PE_V_ADDRESS( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] );
    }
    pd = reinterpret_cast< UINT64 * >( PE_V_ADDRESS( pd_pt[ VIRT_PDPTE_IDX( virtualAddress ) ] ) );
    if ( pd[ VIRT_PDE_IDX( virtualAddress ) ] & PDE_2M ) {
        return PE_V_ADDRESS( pd[ VIRT_PDE_IDX( virtualAddress ) ] );
    }
    pt = reinterpret_cast< UINT64 * >( PE_V_ADDRESS( pd[ VIRT_PDE_IDX( virtualAddress ) ] ) );
    return PE_V_ADDRESS( pt[ VIRT_PTE_IDX( virtualAddress ) ] ) | VIRT_OFFSET( virtualAddress );
}
auto BootServicePage::pmlDump( IN UINT64 *pml ) -> decltype( auto ) {
    if ( pml == NULL ) {
        return;
    }
    UINT64 virtualAddress { };
    UINT64 *pdpt, *pd, *pt;
    for ( UINT16 iPML4E = 0; iPML4E < 512; iPML4E++, pml++ ) {
        if ( ~*pml & PE_P ) {
            virtualAddress += PDPT_SIZE;
            continue;
        }
        pdpt = (UINT64 *)PE_V_ADDRESS( *pml );
        for ( UINT16 pdptIndex { }; pdptIndex < 512; pdptIndex++, pdpt++ ) {
            if ( ~*pdpt & PE_P ) {
                virtualAddress += PD_SIZE;
                continue;
            }
            if ( *pdpt & PDPTE_1G ) {
                virtualAddress += PD_SIZE;
                continue;
            }
            pd = (UINT64 *)PE_V_ADDRESS( *pdpt );
            for ( UINT16 pdeIndex { }; pdeIndex < 512; pdeIndex++, pd++ ) {
                if ( ~*pd & PE_P ) {
                    virtualAddress += PT_SIZE;
                    continue;
                }
                if ( *pd & PDE_2M ) {
                    virtualAddress += PT_SIZE;
                    continue;
                }
                pt = (UINT64 *)PE_V_ADDRESS( *pd );
                for ( UINT16 ptdIndex { }; ptdIndex < 512; ptdIndex++, pt++ ) {
                    if ( ~*pt & PE_P ) {
                        virtualAddress += PAGE_4K;
                        continue;
                    }
                    virtualAddress += PAGE_4K;
                }
            }
        }
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
    BootServiceDataManager< PageConfig > {
        memoryPages
    } {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS,
                   "Initialize the memory pages service management." );
    logger.Close( );
}
//  支持5级分页就是pml5 不然是pml4

auto BootServicePage::setPageTable( ) -> EFI_STATUS {
    // 设置页表
    EFI_STATUS Status { EFI_SUCCESS };
    bool is5LevelPagingSupport = FALSE;
    UINT8 physicalAddressBits { };
    UINT32 pml5EntryCount { 1 };
    UINT32 pml4EntryCount { 1 };
    UINT32 pdpEntryCount { 1 };
    UINT32 tmp { };
    UINT32 eax { }, ecx { };
    _cpuid( 0x7, tmp, tmp, ecx, tmp );     // 查询各个寄存器
    is5LevelPagingSupport = ( ( ecx & ( 1ull << 16 ) ) != 0 );
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
            pml5EntryCount = ( 1ull << ( physicalAddressBits - 48 ) );
        }
        physicalAddressBits = 48;
    }
    if ( physicalAddressBits > 39 ) {
        pml4EntryCount = ( 1ull << ( physicalAddressBits - 39 ) );
        physicalAddressBits = 39;
    }
    pdpEntryCount = ( 1ull << ( physicalAddressBits - 30 ) );
    // 这里可以适当更改(比如像我注释掉的方法)，比如改成AllocatePages的方法，我用的是使用固定地址0x0600000与0xffff800000600000
    // UINT32 kernelPageTableSize { ( ( pdpEntryCount + 1 ) * pml4EntryCount + 1 ) * pml5EntryCount + ( is5LevelPagingSupport ? 1 : 0 ) };
    // VOID *kernelPageTable = AllocatePages( kernelPageTableSize );

    UINT64 pageTableAddress { KERNEL_PAGE_DIRECTORY_PHYSICAL_ADDRESS };
    this->pageTable = pageTableAddress;
    UINT64 *pml5Entry { (UINT64 *)pageTableAddress }, *pml4Entry { }, *pdpEntry { }, *pdEntry { };
    UINT64 pageAddress { };
    if ( is5LevelPagingSupport ) {
        pageTableAddress += PAGE_4K;
    }
    for ( UINT64 pml5Index { }; pml5Index < (UINT64)( pml5EntryCount ); ++pml5Index ) {
        pml4Entry = (UINT64 *)pageTableAddress;
        pageTableAddress += PAGE_4K;
        if ( is5LevelPagingSupport ) {
            this->make_pml( pml5Entry++, pml4Entry, PE_P | PE_RW );
        }
        for ( UINT64 pml4Index { }; pml4Index < (UINT64)( pml5EntryCount == 1 ? pml4EntryCount : 512 ); ++pml4Index ) {
            pdpEntry = (UINT64 *)pageTableAddress;
            pageTableAddress += PAGE_4K;
            this->make_pml( pml4Entry++, pdpEntry, PE_P | PE_RW );
            for ( UINT64 pdpIndex { }; pdpIndex < (UINT64)( pml4EntryCount == 1 ? pdpEntryCount : 512 ); ++pdpIndex ) {
                pdEntry = (UINT64 *)pageTableAddress;
                pageTableAddress += PAGE_4K;
                this->make_pdp( pdpEntry++, pdEntry, PE_P | PE_RW );
                for ( UINT64 pdIndex { }; pdIndex < 512; ++pdIndex ) {
                    this->make_pd( pdEntry++, pageAddress, PE_P | PE_RW | PTE_PAT );
                    pageAddress += PAGE_2M;
                }
            }
        }
    }
    // this->pmlDump( (UINT64 *)this->pageTable );
    this->updateCr3( );
    Status = displayStep( );
    return Status;
}
auto BootServicePage::make_pd( OUT UINT64 *pdEntry, IN UINT64 pageTableAddress, IN CONST UINT32 flags ) -> VOID {
    *pdEntry = pageTableAddress | flags;
}
auto BootServicePage::make_pdp( OUT UINT64 *pdpEntry, IN UINT64 *pdEntry, IN CONST UINT32 flags ) -> VOID {
    *pdpEntry = (UINT64)pdEntry | flags;
}
auto BootServicePage::make_pml( OUT UINT64 *pmlEntry, IN UINT64 *pdpEntry, IN CONST UINT32 flags ) -> VOID {
    *pmlEntry = (UINT64)pdpEntry | flags;
}
}     // namespace QuantumNEC::Boot