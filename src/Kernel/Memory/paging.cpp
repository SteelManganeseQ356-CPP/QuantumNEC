#include <Kernel/Memory/paging.hpp>
#include <Lib/Base/allocate.hpp>
#include <Lib/IO/Stream/iostream>
#include <Lib/STL/string>
#include <Utils/asm.hpp>
using namespace QuantumNEC::Lib::IO;

PUBLIC namespace {
    using namespace QuantumNEC;
    using namespace QuantumNEC::Kernel::Memory;

    Lib::Base::AllocateTable globalFreePageDescriptorTable { };
    Lib::Base::AllocateTableEntry pageDescriptorEntry[ PAGE_DESCRIPTOR_ENTRY_COUNT ] { };
}

PUBLIC namespace QuantumNEC::Kernel::Memory {
    PageMemoryManagement::PageMemoryManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) {
        Lib::Types::uint64_t memoryTotal { };
        Lib::Types::Ptr< Lib::Types::EfiMemoryDescriptor > efiMemoryDescriptor { _config->MemoryData.Buffer };
        auto judgmentMemoryType = [ &, this ]( Lib::Types::size_t n ) -> MemoryPageAttribute {
            switch ( efiMemoryDescriptor[ n ].Type ) {
            case Lib::Types::EfiMemoryType::EfiConventionalMemory:
            case Lib::Types::EfiMemoryType::EfiBootServicesCode:
            case Lib::Types::EfiMemoryType::EfiBootServicesData:
            case Lib::Types::EfiMemoryType::EfiLoaderCode:
            case Lib::Types::EfiMemoryType::EfiLoaderData:
                return MemoryPageAttribute::FREE_MEMORY;
            case Lib::Types::EfiMemoryType::EfiRuntimeServicesCode:
            case Lib::Types::EfiMemoryType::EfiRuntimeServicesData:
            case Lib::Types::EfiMemoryType::EfiMemoryMappedIO:
            case Lib::Types::EfiMemoryType::EfiMemoryMappedIOPortSpace:
            case Lib::Types::EfiMemoryType::EfiPalCode:
            case Lib::Types::EfiMemoryType::EfiReservedMemoryType:
                return MemoryPageAttribute::RESERVED_MEMORY;
            case Lib::Types::EfiMemoryType::EfiACPIReclaimMemory:
                return MemoryPageAttribute::ACPI_MEMORY;
            case Lib::Types::EfiMemoryType::EfiACPIMemoryNVS:
                return MemoryPageAttribute::ACPI_MEMORY_NVS;
            case Lib::Types::EfiMemoryType::EfiUnusableMemory:
            case Lib::Types::EfiMemoryType::EfiMaxMemoryType:
                return MemoryPageAttribute::UNUSEABLE_MEMORY;
            default:
                return MemoryPageAttribute::UNDEF_MEMORY;
            }
        };
        for ( Lib::Types::size_t i { }; i < _config->MemoryData.MemoryCount; ++i ) {
            if ( judgmentMemoryType( i ) == MemoryPageAttribute::FREE_MEMORY ) {
                Lib::Types::uint64_t
                    start { efiMemoryDescriptor[ i ].PhysicalStart },
                    end { start + ( efiMemoryDescriptor[ i ].NumberOfPages << 12 ) };
                Lib::Types::uint64_t j { };
                for ( j = i + 1; j < _config->MemoryData.MemoryCount; ++j ) {
                    if ( judgmentMemoryType( i ) != MemoryPageAttribute::FREE_MEMORY || end != efiMemoryDescriptor[ j ].PhysicalStart )
                        break;
                    end += efiMemoryDescriptor[ j ].NumberOfPages << 12;
                }
                i = j - 1;
                // 对齐到2MB
                start = divRoundUp( start, PAGE_SIZE );
                if ( start < 5 )
                    start = 5;
                end = end / PAGE_SIZE;
                if ( Lib::Base::AllocateManagement allocate { &globalFreePageDescriptorTable, pageDescriptorEntry, MEMORY_ALLOCATE_DESCRIPTOR }; end > start && end - start >= 1 ) {
                    memoryTotal += ( end - start ) * PAGE_SIZE;
                    allocate.free( start, end - start );
                }
            }
        }
        memoryTotal = memoryTotal / 1_MB;
        sout[ ostream::HeadLevel::SYSTEM ] << "OS Can Use Memory : " << memoryTotal << " MB";
        memoryTotal = memoryTotal / 1_KB;
        if ( memoryTotal ) {
            sout << " <=> " << memoryTotal << " GB";
        }
        endl( sout );
    }
    auto PageMemoryManagement::malloc( IN Lib::Types::size_t size )->Lib::Types::Ptr< VOID > {
        return size ? reinterpret_cast< Lib::Types::Ptr< VOID > >( 0UL + Lib::Base::AllocateManagement { &globalFreePageDescriptorTable, pageDescriptorEntry, MEMORY_ALLOCATE_DESCRIPTOR }.alloc( size ) * PAGE_SIZE ) : NULL;
    }
    auto PageMemoryManagement::free( IN Lib::Types::Ptr< VOID > address, IN Lib::Types::size_t size )->VOID {
        if ( !size || !address || ( ( reinterpret_cast< Lib::Types::uint64_t >( address ) & 0x1fffff ) ) )
            return;
        Lib::Base::AllocateManagement { &globalFreePageDescriptorTable, pageDescriptorEntry, MEMORY_ALLOCATE_DESCRIPTOR }.free( reinterpret_cast< Lib::Types::uint64_t >( address ) / PAGE_SIZE, size );
    }
}