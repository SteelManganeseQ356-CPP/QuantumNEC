#include <Arch/x86_64/platform/platform.hpp>
#include <Kernel/memory.hpp>
using namespace QuantumNEC::Kernel::Memory;
auto operator new( IN unsigned long int, IN QuantumNEC::Lib::Types::Ptr< VOID > addr ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID > {
    return addr;
}
auto operator new( IN unsigned long int size ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID > {
    return physical_to_virtual( HeapMemoryManagement::malloc( size ) );
}
auto operator new[]( IN unsigned long int size ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID > {
    return physical_to_virtual( HeapMemoryManagement::malloc( size ) );
}
auto operator delete( IN QuantumNEC::Lib::Types::Ptr< VOID >, IN unsigned long int ) noexcept -> VOID {
}
auto operator delete( IN QuantumNEC::Lib::Types::Ptr< VOID > address ) noexcept -> VOID {
    HeapMemoryManagement::free( virtual_to_physical( address ) );
}
auto operator delete[]( IN QuantumNEC::Lib::Types::Ptr< VOID > address ) noexcept -> VOID {
    HeapMemoryManagement::free( virtual_to_physical( address ) );
}
auto operator delete[]( IN QuantumNEC::Lib::Types::Ptr< VOID >, IN unsigned long int ) noexcept -> VOID {
}