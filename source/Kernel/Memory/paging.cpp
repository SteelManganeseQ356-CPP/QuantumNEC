#include <Kernel/Memory/paging.hpp>
#include <Lib/IO/Stream/iostream>
#include <Lib/STL/string>
#include <Utils/asm.hpp>
PUBLIC namespace {
    PUBLIC using namespace QuantumNEC::Lib::IO;
    PUBLIC using namespace QuantumNEC;
    PUBLIC using namespace QuantumNEC::Kernel::Memory;
    PRIVATE inline constexpr Lib::Types::size_t operator""_KB( IN CONST Lib::Types::size_t kib ) {
        return kib * 1024;
    }

    PRIVATE inline constexpr Lib::Types::size_t operator""_MB( IN CONST Lib::Types::size_t mib ) {
        return mib * 1024_KB;
    }

    PRIVATE inline constexpr Lib::Types::size_t operator""_GB( IN CONST Lib::Types::size_t gib ) {
        return gib * 1024_MB;
    }
}

PUBLIC namespace QuantumNEC::Kernel::Memory {
    PageMemoryManagement::PageMemoryManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) {
        Lib::Types::uint64_t memoryTotal { };                                                                      // 计算可用内存数量
        Lib::Types::Ptr< Lib::Types::EfiMemoryDescriptor > efiMemoryDescriptor { _config->MemoryData.Buffer };     // memory map

        auto checkMemoryType = [ &, this ]( Lib::Types::size_t n ) -> MemoryPageAttribute {
            switch ( efiMemoryDescriptor[ n ].Type ) {
            case Lib::Types::EfiMemoryType::EfiConventionalMemory:
            case Lib::Types::EfiMemoryType::EfiBootServicesCode:
            case Lib::Types::EfiMemoryType::EfiBootServicesData:
            case Lib::Types::EfiMemoryType::EfiLoaderCode:
            case Lib::Types::EfiMemoryType::EfiLoaderData:
                return MemoryPageAttribute::FREE_MEMORY;     // 可用空闲内存
            case Lib::Types::EfiMemoryType::EfiRuntimeServicesCode:
            case Lib::Types::EfiMemoryType::EfiRuntimeServicesData:
            case Lib::Types::EfiMemoryType::EfiMemoryMappedIO:
            case Lib::Types::EfiMemoryType::EfiMemoryMappedIOPortSpace:
            case Lib::Types::EfiMemoryType::EfiPalCode:
            case Lib::Types::EfiMemoryType::EfiReservedMemoryType:
                return MemoryPageAttribute::RESERVED_MEMORY;     // 保留内存
            case Lib::Types::EfiMemoryType::EfiACPIReclaimMemory:
                return MemoryPageAttribute::ACPI_MEMORY;     // ACPI 保留内存
            case Lib::Types::EfiMemoryType::EfiACPIMemoryNVS:
                return MemoryPageAttribute::ACPI_MEMORY_NVS;     // ACPI 可回收内存
            case Lib::Types::EfiMemoryType::EfiUnusableMemory:
            case Lib::Types::EfiMemoryType::EfiMaxMemoryType:
                return MemoryPageAttribute::UNUSEABLE_MEMORY;     // 不可用内存
            default:
                return MemoryPageAttribute::UNDEF_MEMORY;     // 其他未定义内存
            }
        };
        for ( Lib::Types::size_t i { }; i < _config->MemoryData.MemoryCount; ++i ) {
            auto type { checkMemoryType( i ) };
            if ( type == MemoryPageAttribute::FREE_MEMORY ) {
                // 如果为空闲内存
                Lib::Types::uint64_t
                    start { efiMemoryDescriptor[ i ].PhysicalStart },
                    end { start + ( efiMemoryDescriptor[ i ].NumberOfPages << 12 ) };
                // 计算内存页边界
                Lib::Types::uint64_t j { };
                for ( j = i + 1; j < _config->MemoryData.MemoryCount; ++j ) {
                    if ( type != MemoryPageAttribute::FREE_MEMORY || end != efiMemoryDescriptor[ j ].PhysicalStart )
                        break;
                    end += efiMemoryDescriptor[ j ].NumberOfPages << 12;
                }
                sout[ ostream::HeadLevel::DEBUG ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "The start address of the page -> %#018lx | end address of the page -> %#018lx\n\r", start, end );
                i = j - 1;
                // 对齐到2MB
                start = divRoundUp( start, PAGE_SIZE );
                if ( start < 5 )
                    start = 5;
                end = end / PAGE_SIZE;
                if ( Lib::Base::AllocateManagement allocate { &this->globalFreePageDescriptorTable, this->pageDescriptorEntry, MEMORY_ALLOCATE_DESCRIPTOR }; end > start && end - start >= 1 ) {
                    memoryTotal += ( end - start ) * PAGE_SIZE;
                    allocate.free( start, end - start );
                }
            }
            else if ( type == MemoryPageAttribute::RESERVED_MEMORY ) {
            }
            else if ( type == MemoryPageAttribute::ACPI_MEMORY ) {
            }
            else if ( type == MemoryPageAttribute::ACPI_MEMORY_NVS ) {
            }
            else if ( type == MemoryPageAttribute::UNUSEABLE_MEMORY ) {
            }
            else if ( type == MemoryPageAttribute::UNDEF_MEMORY ) {
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