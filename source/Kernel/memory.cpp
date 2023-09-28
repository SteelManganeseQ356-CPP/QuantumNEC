#include <Kernel/memory.hpp>

PUBLIC namespace QuantumNEC::Kernel {
    MemoryManagement::MemoryManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) :
        Memory::PageMemoryManagement { _config },
        Memory::HeapMemoryManagement { _config },
        Memory::MemoryMapManagement { _config },
        Memory::StackMemoryManagement { } {
            
    }
}