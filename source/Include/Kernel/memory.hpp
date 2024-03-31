#pragma once
#include <Kernel/Memory/heap.hpp>
#include <Kernel/Memory/map.hpp>
#include <Kernel/Memory/paging.hpp>
#include <Kernel/Memory/stack.hpp>
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Kernel {
    PUBLIC class MemoryManagement :
        Memory::PageMemoryManagement,
        Memory::HeapMemoryManagement,
        Memory::MemoryMapManagement,
        Memory::StackMemoryManagement
    {
    public:
        explicit( true ) MemoryManagement( Lib::Types::Ptr< Lib::Types::BootConfig > ) noexcept( true );

    public:
        virtual ~MemoryManagement( VOID ) noexcept( true ) = default;

    private:
    };
}