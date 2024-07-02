#pragma once
#include <Kernel/Memory/heap.hpp>
#include <Kernel/Memory/map.hpp>
#include <Kernel/Memory/paging.hpp>
#include <Kernel/Memory/stack.hpp>
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Kernel {
    PUBLIC class Memory :
        PageMemory,
        MemoryMap,
        HeapMemory,
        StackMemory
    {
    public:
        explicit Memory( Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept;

    public:
        virtual ~Memory( VOID ) noexcept = default;

    private:
    };
}