#include <Kernel/Memory/heap.hpp>
#include <Kernel/Memory/paging.hpp>
#include <Lib/STL/string>
PUBLIC namespace {
    using namespace QuantumNEC::Kernel::Memory;
    HeapMemoryManagement::MemoryBlock memoryBlock[ MEMORY_BLOCK_KIND ] { };
}
PUBLIC namespace QuantumNEC::Kernel::Memory {
    HeapMemoryManagement::HeapMemoryManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > ) {
        Lib::Types::int32_t blockSize { MIN_MEMORY_SIZE };
        for ( Lib::Types::size_t i { }; i < MEMORY_BLOCK_KIND; i++ ) {
            memoryBlock[ i ].size = blockSize;
            memoryBlock[ i ].flags = FALSE;
            Lib::STL::list_init( &memoryBlock[ i ].freeBlockTable );
            blockSize *= 2;
        }
        return;
    }
    auto HeapMemoryManagement::malloc( IN Lib::Types::size_t size )->Lib::Types::Ptr< VOID > {
        Lib::Types::int32_t index { };
        Lib::Types::Ptr< Zone > zone { };
        Lib::Types::Ptr< Lib::STL::ListNode > block;
        if ( size > MAX_MEMORY_SIZE ) {
            Lib::Types::size_t pageCount { };
            pageCount = divRoundUp( size + sizeof *zone, PAGE_SIZE );
            zone = reinterpret_cast< Lib::Types::Ptr< Zone > >( PageMemoryManagement::malloc( pageCount ) );
            if ( !zone ) {
                return NULL;
            }
            zone->block = NULL;
            zone->count = pageCount;
            zone->flags = TRUE;
            return zone + 1;
        }
        for ( Lib::Types::size_t i { }; i < MEMORY_BLOCK_KIND; i++ ) {
            if ( size <= memoryBlock[ i ].size ) {
                break;
            }
        }

        if ( Lib::STL::list_is_empty( &memoryBlock[ index ].freeBlockTable ) ) {
            zone = reinterpret_cast< Lib::Types::Ptr< Zone > >( PageMemoryManagement::malloc( 1 ) );
            if ( !zone ) {
                return NULL;
            }
            Lib::STL::memset( zone, 0, PAGE_SIZE );
            zone->block = &memoryBlock[ index ];
            zone->flags = FALSE;
            zone->blockCount = ( PAGE_SIZE - sizeof *zone - ( PAGE_SIZE - sizeof *zone ) % zone->block->size ) / zone->block->size;
            zone->count = zone->blockCount;
            Lib::Types::size_t blockIndex { };
            // intr_status_t intr_status = intr_disable( );
            auto zoneToBlock = [ & ]( int idx ) -> Lib::Types::Ptr< Lib::STL::ListNode > {
                Lib::Types::uint64_t addr = (Lib::Types::uint64_t)zone + sizeof( *zone ) + ( PAGE_SIZE - sizeof( *zone ) ) % zone->block->size;
                return reinterpret_cast< Lib::Types::Ptr< Lib::STL::ListNode > >( addr + ( idx * ( zone->block->size ) ) );
            };

            for ( blockIndex = 0; blockIndex < zone->count; blockIndex++ ) {
                block = zoneToBlock( blockIndex );
                Lib::STL::list_add_to_end( &zone->block->freeBlockTable, block );
            }
            //   intr_set_status( intr_status );
        }
        block = Lib::STL::list_pop( &memoryBlock[ index ].freeBlockTable );
        memset( block, 0, memoryBlock[ index ].size );
        zone = reinterpret_cast< Lib::Types::Ptr< Zone > >( reinterpret_cast< Lib::Types::uint64_t >( block ) & 0xffffffffffe00000 );
        zone->count--;
        return block;
    }

    auto HeapMemoryManagement::free( IN Lib::Types::Ptr< VOID > address )->VOID {
        if ( !address ) {
            return;
        }
        Lib::Types::Ptr< Lib::STL::ListNode > block { reinterpret_cast< Lib::Types::Ptr< Lib::STL::ListNode > >( Virt_To_Phy( address ) ) };
        Lib::Types::Ptr< Zone > zone { reinterpret_cast< Lib::Types::Ptr< Zone > >( reinterpret_cast< Lib::Types::uint64_t >( block ) & 0xffffffffffe00000 ) };     // 记录一片内存区域
        if ( !zone->block && zone->flags ) {
            PageMemoryManagement::free( zone, zone->count );
            return;
        }
        Lib::STL::list_add_to_end( &zone->block->freeBlockTable, block );
        zone->count++;
        if ( zone->count == zone->blockCount ) {
            for ( Lib::Types::size_t index { }; index < zone->blockCount; ++index ) {
                block = reinterpret_cast< Lib::Types::Ptr< Lib::STL::ListNode > >( ( reinterpret_cast< Lib::Types::uint64_t >( zone ) + sizeof *zone + ( PAGE_SIZE - sizeof *zone ) % zone->block->size ) + index * zone->block->size );
                Lib::STL::list_delete( block );
            }
            PageMemoryManagement::free( zone, 1 );
            return;
        }
        return;
    }
}
