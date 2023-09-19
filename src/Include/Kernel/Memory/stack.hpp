#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Kernel::Memory {
    PUBLIC class StackMemoryManagement
    {
    public:
        explicit( true ) StackMemoryManagement( VOID ) noexcept( true );
        virtual ~StackMemoryManagement( VOID ) noexcept( true ) = default;

    public:
        STATIC auto malloc( IN Lib::Types::size_t ) -> Lib::Types::Ptr< VOID >;
        STATIC auto free( IN Lib::Types::Ptr< VOID > ) -> VOID;
    };
}     // namespace QuantumNEC::Kernel::Memory
