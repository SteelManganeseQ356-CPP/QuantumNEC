#pragma once
#include <Boot/Data.hpp>

namespace QuantumNEC::Boot {
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
}     // namespace QuantumNEC::Boot