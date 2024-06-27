#include "Kernel/Memory/heap.hpp"
#include <Arch/x86_64/platform/platform.hpp>
#include <Kernel/memory.hpp>
#include <Lib/Types/Uefi.hpp>
using namespace QuantumNEC::Kernel;
auto operator new( IN unsigned long int, IN QuantumNEC::Lib::Types::Ptr< VOID > addr ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID > {
    return addr;
}
auto operator new( IN unsigned long int size ) -> QuantumNEC::Lib::Types::Ptr< VOID > {
    return physical_to_virtual( HeapMemory::malloc( size ) );
}
auto operator new[]( IN unsigned long int size ) -> QuantumNEC::Lib::Types::Ptr< VOID > {
    return physical_to_virtual( HeapMemory::malloc( size ) );
}
auto operator delete( IN QuantumNEC::Lib::Types::Ptr< VOID >, IN unsigned long int ) noexcept -> VOID {
}
auto operator delete( IN QuantumNEC::Lib::Types::Ptr< VOID > address ) noexcept -> VOID {
    HeapMemory::free( virtual_to_physical( address ) );
}
auto operator delete[]( IN QuantumNEC::Lib::Types::Ptr< VOID > address ) noexcept -> VOID {
    HeapMemory::free( virtual_to_physical( address ) );
}
auto operator delete[]( IN QuantumNEC::Lib::Types::Ptr< VOID >, IN unsigned long int ) noexcept -> VOID {
}