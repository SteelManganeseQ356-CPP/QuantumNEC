#include <Kernel/memory.hpp>
using namespace QuantumNEC::Kernel;
auto operator new( IN unsigned long int, IN VOID *addr ) noexcept -> VOID * {
    return addr;
}
auto operator new( IN unsigned long int size ) -> VOID * {
    return physical_to_virtual( HeapMemory::malloc( size ) );
}
auto operator new[]( IN unsigned long int size ) -> VOID * {
    return physical_to_virtual( HeapMemory::malloc( size ) );
}
auto operator delete( IN VOID *, IN unsigned long int ) noexcept -> VOID {
}
auto operator delete( IN VOID *address ) noexcept -> VOID {
    HeapMemory::free( virtual_to_physical( address ) );
}
auto operator delete[]( IN VOID *address ) noexcept -> VOID {
    HeapMemory::free( virtual_to_physical( address ) );
}
auto operator delete[]( IN VOID *, IN unsigned long int ) noexcept -> VOID {
}