#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
#include <Lib/STL/list>
PUBLIC namespace QuantumNEC::Kernel::Memory {
    // 所支持分配的最小内存块大小
    PUBLIC constexpr CONST auto MIN_MEMORY_SIZE { 4096 };
    // 所支持分配的最大内存块大小,超过这个数就直接分配整页
    PUBLIC constexpr CONST auto MAX_MEMORY_SIZE { 0x40000 };

    PUBLIC constexpr CONST auto MEMORY_BLOCK_KIND { 7 };
    PUBLIC class HeapMemoryManagement
    {
    public:
        // 不同大小和类型的内存组
        typedef struct
        {
            Lib::Types::size_t size;            /* 内存块单元的大小 */
            Lib::Types::BOOL flags;           /* 标志 */
            Lib::STL::ListTable freeBlockTable; /* 组中所有的内存块 */
        } MemoryBlock;                          // 内存块类型
    private:
        typedef struct
        {
            Lib::Types::Ptr< MemoryBlock > block;     // 该内存区域属于哪个内存块类型
            Lib::Types::uint64_t blockCount;          // 记录块的数量
            Lib::Types::size_t count;
            Lib::Types::BOOL flags;
        } Zone;     // 内存区域类型
    public:
    public:
        explicit HeapMemoryManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true );

    public:
        virtual ~HeapMemoryManagement( VOID ) noexcept( true ) = default;

    public:
        STATIC auto malloc( IN Lib::Types::size_t size ) -> Lib::Types::Ptr< VOID >;
        STATIC auto free( IN Lib::Types::Ptr< VOID > address ) -> VOID;
    };
}