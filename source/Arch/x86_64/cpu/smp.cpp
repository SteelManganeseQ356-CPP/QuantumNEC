#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/STL/cstring>
#include <Utils/asm.hpp>
PUBLIC namespace QuantumNEC::Architecture::CPU {
    TaskStateSegmentDescriptorManagement::TaskStateSegmentDescriptorManagement( IN Lib::Types::Ptr< VOID > _gdt ) {
        // 设置TSS
        Lib::Types::Ptr< SegmentDescriptor > gdt { reinterpret_cast< Lib::Types::Ptr< SegmentDescriptor > >( _gdt ) };
        Lib::Types::uint32_t tss_size = sizeof( TaskStateSegmentDescriptor64 );
        Lib::STL::memset( &tss, 0, tss_size );
        tss.io_map = tss_size << 16;
        Lib::Types::uint64_t tss_base_l { ( (Lib::Types::uint64_t)&tss ) & 0xffffffff };
        Lib::Types::uint64_t tss_base_h { ( ( (Lib::Types::uint64_t)&tss ) >> 32 ) & 0xffffffff };
        GlobalDescriptorManagement::makeDescriptor( gdt, 9, ( Lib::Types::uint32_t )( tss_base_l & 0xffffffff ), tss_size - 1, Platform::AR_TSS64 );
        Lib::STL::memcpy( &gdt[ 9 + 1 ], &tss_base_h, 8 );
    }
    auto TaskStateSegmentDescriptorManagement::set( VOID ) CONST->VOID {
        // 载入TSS
        ASM(
            "movw %%ax,%%ds \n\t"
            "movw %%ax,%%es \n\t"
            "movw %%ax,%%fs \n\t"
            "movw %%ax,%%gs \n\t"
            "movw %%ax,%%ss \n\t"
            "pushq %[SELECTOR_CODE64] \n\t"
            "leaq .next(%%rip),%%rax \n\t"
            "pushq %%rax \n\t"
            "lretq \n\r"
            ".next: \n\t"
            "ltr %w[TSS] \n\t"
            :
            : [SELECTOR_CODE64] "i"( Platform::SELECTOR_CODE64_K ),
              [SELECTOR_DATA64] "ax"( Platform::SELECTOR_DATA64_K ),
              [TSS] "r"( Platform::SELECTOR_TSS )
            : );
    }
}