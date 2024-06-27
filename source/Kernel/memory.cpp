#include <Kernel/memory.hpp>

PUBLIC namespace QuantumNEC::Kernel {
    Memory::Memory( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept :
        Memory::PageMemory { _config },
        Memory::HeapMemory { _config },
        Memory::MemoryMap { _config },
        Memory::StackMemory { } {
    }
}