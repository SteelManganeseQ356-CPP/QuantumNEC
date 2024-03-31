#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
#include <Lib/STL/list>
PUBLIC namespace QuantumNEC::Kernel::Memory {
    // 内存块类型
    PUBLIC constexpr CONST auto MEMORY_BLOCK_KIND { 15 };
    // 内存块魔术字节
    PUBLIC constexpr CONST auto MEMORY_BLOCK_MAGIC_NUMBER { 0x1145141919810ull };
    PUBLIC class HeapMemoryManagement
    {
    private:
        // // 不同大小和类型的内存组
        // typedef struct
        // {
        //     Lib::Types::size_t size;         /* 内存块单元的大小 */
        //     Lib::Types::size_t total_free;   /* 剩余内存 */
        //     Lib::STL::ListTable block_table; /* 组中所有的内存块 */
        // } MemoryBlock;                       // 内存块类型
        // typedef struct
        // {
        //     Lib::Types::Ptr< MemoryBlock > block;     // 该内存区域属于哪个内存块类型
        //     Lib::Types::uint64_t block_count;         // 记录块的数量
        //     Lib::Types::uint64_t zone_count;          // 记录区域的数量
        //     Lib::Types::BOOL flags;                   // 大内存区域还是小内存区域
        // } Zone;                                       // 内存区域类型

        typedef struct
        {
            Lib::STL::ListNode zone_node;
            Lib::STL::ListTable block_table;
            Lib::Types::uint64_t block_count;     // 记录块的数量
            Lib::Types::Ptr< VOID > zone_start_address;
            Lib::Types::Ptr< VOID > zone_end_address;
            Lib::Types::uint64_t size;
            Lib::Types::BOOL is_fill;
        } Zone;
        typedef struct
        {
            Lib::STL::ListNode block_node;
            Lib::Types::Ptr< Zone > owner;
            Lib::Types::Ptr< VOID > start_address;
            Lib::Types::Ptr< VOID > end_address;
            Lib::Types::uint64_t size;
            Lib::Types::BOOL is_free;
            Lib::Types::uint64_t block_magic;
        } Block;

    public:
        explicit HeapMemoryManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true );

    public:
        virtual ~HeapMemoryManagement( VOID ) noexcept( true ) = default;

    public:
        STATIC auto malloc( IN Lib::Types::size_t size ) -> Lib::Types::Ptr< VOID >;
        STATIC auto free( IN Lib::Types::Ptr< VOID > address ) -> VOID;

    private:
        // inline STATIC HeapMemoryManagement::MemoryBlock memory_block_table[ MEMORY_BLOCK_KIND ] { };     // 对应UEFI里的不同内存类型//

        inline STATIC Lib::STL::ListTable global_memory_zone_table { };
    };
}