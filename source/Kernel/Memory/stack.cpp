#include <Kernel/Memory/stack.hpp>
PUBLIC namespace QuantumNEC::Kernel::Memory {
    
    StackMemoryManagement::StackMemoryManagement( VOID ) {
    }
    auto StackMemoryManagement::malloc( IN Lib::Types::size_t _size )->Lib::Types::Ptr< VOID > {
        if ( !_size ) return NULL;
        return nullptr;
    }
    auto StackMemoryManagement::free( IN Lib::Types::Ptr< VOID > )->VOID {
    }
}