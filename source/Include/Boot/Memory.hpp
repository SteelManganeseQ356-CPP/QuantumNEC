#pragma once
#include <Boot/Data.hpp>

namespace QuantumNEC::Boot {
constexpr CONST auto PF_X { 1 << 0 };
constexpr CONST auto PF_W { 1 << 1 };
constexpr CONST auto PF_R { 1 << 2 };
constexpr CONST auto PE_P { 1 << 0 };
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
constexpr CONST auto PAGE_4K { 0x1000ULL };
constexpr CONST auto PAGE_2M { 0x200000ULL };
constexpr CONST auto PT_SIZE { PAGE_4K * 512ULL };
constexpr CONST auto PD_SIZE { PAGE_4K * 512ULL * 512ULL };
constexpr CONST auto PDPT_SIZE { PAGE_4K * 512ULL * 512ULL * 512ULL };
constexpr CONST auto PML_SIZE { PAGE_4K * 512ULL * 512ULL * 512ULL * 512ULL };

/**
 * @brief 值
 */
constexpr auto PE_V_FLAGS( auto Entry ) -> UINT64 {
    return ( (UINT64)Entry ) & 0x7FF;
}
constexpr auto PE_V_ADDRESS( auto Entry ) -> UINT64 {
    return ( (UINT64)Entry ) & ~0x7FF;
}
/**
 * @brief 设置
 */
constexpr auto PE_S_FLAGS( auto Flags ) -> UINT64 {
    return (UINT64)( Flags & 0x7FF );
}
constexpr auto PE_S_ADDRESS( auto Address ) -> UINT64 {
    return (UINT64)( ( (UINT64)Address & ~0x7FF ) );
}
/**
 *  @brief 通过虚拟地址查找条目或其他条目
 */
constexpr auto VIRT_PML4E_IDX( auto Address ) -> UINT64 {
    return ( ( (UINT64)Address >> 39 ) & 0x1FF );
}
constexpr auto VIRT_PDPTE_IDX( auto Address ) -> UINT64 {
    return ( ( (UINT64)Address >> 30 ) & 0x1FF );
}
constexpr auto VIRT_PDE_IDX( auto Address ) -> UINT64 {
    return ( ( (UINT64)Address >> 21 ) & 0x1FF );
}
constexpr auto VIRT_PTE_IDX( auto Address ) -> UINT64 {
    return ( ( (UINT64)Address >> 12 ) & 0x1FF );
}
/**
 *  @brief 页内偏移
 */
constexpr auto VIRT_OFFSET( auto Address ) -> UINT64 {
    return ( (UINT64)Address & 0xFFF );
}
/**
 *  @brief 根据PageSize来转换单位(大小->页)
 */
constexpr decltype( auto ) SIZE_TO_PAGES( auto Size, auto PageSize ) {
    return ( !( Size % PageSize ) ? Size / PageSize : ( Size - Size % PageSize ) / PageSize + 1 );
}

typedef struct
{
    EFI_MEMORY_DESCRIPTOR *Buffer;
    UINTN MemorySize;
    UINTN MemoryCount;
    UINTN MemoryKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} MemoryConfig;
typedef struct
{
    UINT8 Valid;
    UINT32 Flags;
    UINT64 MemorySize;
    PHYSICAL_ADDRESS PhysicsAddress;
    PHYSICAL_ADDRESS VirtualAddress;

    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} _packed PageConfig;
class BootServiceMemory : protected BootServiceDataManager< MemoryConfig >
{
public:
    explicit BootServiceMemory( IN MemoryConfig * );
    virtual ~BootServiceMemory( VOID ) = default;

public:
    /**
     * @brief 获取MemoryMap
     * @return EFI_STATUS
     */
    auto getMemoryMap( VOID ) -> EFI_STATUS;
};
class BootServicePage : public BootServiceDataManager< PageConfig >
{
private:
    enum class MemoryMode {
        MEMORY_4K = 0x0,
        MEMORY_2M = 0x1,
        MEMORY_1G = 0x2,
        MEMORY_UNDERSCORE = 0x3,
    };

public:
    explicit BootServicePage( IN PageConfig * );
    virtual ~BootServicePage( VOID ) = default;

private:
    /**
     * @brief 消除页保护
     * @param status 状态
     */
    auto PageTableProtect( bool status ) -> decltype( auto ) {
        if ( status )
            AsmWriteCr0( AsmReadCr0( ) | 0x00010000 );
        else
            AsmWriteCr0( AsmReadCr0( ) & ~0x00010000 );
        return;
    }
    auto isPageMapped( IN UINT64 pml4TableAddress, IN UINT64 virtualAddress ) -> decltype( auto );
    auto map( IN UINT64 pml4TableAddress, IN UINT64 physicsAddress,
              IN UINT64 virtualAddress, IN UINT64 size,
              IN UINT64 flags, IN MemoryMode mode ) -> decltype( auto );
    /**
     * @brief 虚拟地址转换为物理地址
     */
    auto VTPAddress( IN UINT64 pml4TableAddress, IN UINT64 virtualAddress ) -> decltype( auto );
    auto pmlDump( IN UINT64 *pml ) -> decltype( auto );

public:
    /**
     * @brief 更新CR3寄存器
     */
    auto updateCr3( VOID ) -> UINTN;
    /**
     * @brief 设置页表
     */
    auto setPageTable( ) -> EFI_STATUS;

private:
    auto make_pd( OUT UINT64 *pdEntry, IN UINT64 pageTableAddress, IN CONST UINT32 flags ) -> VOID;
    auto make_pdp( OUT UINT64 *pdpEntry, IN UINT64 *pdEntry, IN CONST UINT32 flags ) -> VOID;
    auto make_pml( OUT UINT64 *pmlEntry, IN UINT64 *pdpEntry, IN CONST UINT32 flags ) -> VOID;

private:
    UINT64 pageTable { };
};
}     // namespace QuantumNEC::Boot