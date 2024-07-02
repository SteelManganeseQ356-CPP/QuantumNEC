#include "Lib/Types/type_ptr.hpp"
#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/Base/deflib.hpp>
#include <Lib/IO/Stream/iostream>
#include <Kernel/task.hpp>

PUBLIC namespace QuantumNEC::Architecture {
    CPUs::CPUs( VOID ) noexcept :
        InterruptDescriptorManager { idt, Architecture::INTERRUPT_DESCRIPTOR_COUNT },
        GlobalSegmentDescriptorManager { gdt, Architecture::SEGMENT_DESCRIPTOR_COUNT } {
        using namespace QuantumNEC::Lib::IO;
        using namespace QuantumNEC::Lib::Types;
        // CPU Management初始化
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the CPU management." << Lib::IO::endl;
        using enum DisplayColor;
        uint32_t cpuFacName[ 4 ] { };
        char_t factoryName[ 17 ] { };
        this->cpuid( 0, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 0 ] ) = cpuFacName[ 1 ];
        *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 4 ] ) = cpuFacName[ 3 ];
        *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 8 ] ) = cpuFacName[ 2 ];
        factoryName[ 12 ] = '\0';
        sout[ ostream::HeadLevel::INFO ] << "CPU device information is as follows:" << endl;
        sout[ ostream::HeadLevel::SYSTEM ].printk( WHITE, BLACK, "%s\t%#010x\t%#010x\t%#010x\n", factoryName, cpuFacName[ 1 ], cpuFacName[ 3 ], cpuFacName[ 2 ] );
        // 获取处理器商标信息
        sout[ ostream::HeadLevel::SYSTEM ];
        for ( uint32_t i { 0x80000002 }; i <= 0x80000004; i++ ) {
            this->cpuid( i, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
            *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 0 ] ) = cpuFacName[ 0 ];
            *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 4 ] ) = cpuFacName[ 1 ];
            *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 8 ] ) = cpuFacName[ 2 ];
            *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 12 ] ) = cpuFacName[ 3 ];
            factoryName[ 16 ] = '\0';
            sout.printk( WHITE, BLACK, "%s", factoryName );
        }
        endl( sout );
        // 获取处理器的版本信息
        this->cpuid( 1, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        sout[ ostream::HeadLevel::SYSTEM ].printk( WHITE, BLACK,
                                                   "Family Code -> %#010x, Extended Family -> %#010x | Model Number:%#010x | Extended Model -> %#010x | Processor Type -> %#010x | Stepping ID -> %#010x\n",
                                                   ( cpuFacName[ 0 ] >> 8 & 0xf ),       // 处理器商标信息引索值(IA-32处理器特有)
                                                   ( cpuFacName[ 0 ] >> 20 & 0xff ),     // CLFLUSH指令刷新的缓存行容量(单位8B)
                                                   ( cpuFacName[ 0 ] >> 4 & 0xf ),       // 处理器包内最大可寻址逻辑处理器ID
                                                   ( cpuFacName[ 0 ] >> 16 & 0xf ),      // 初始 APIC ID
                                                   ( cpuFacName[ 0 ] >> 12 & 0x3 ),
                                                   ( cpuFacName[ 0 ] & 0xf ) );     // 上面两个都是处理器支持的位功能信息
        // 获取线性/物理地址位宽
        this->cpuid( 0x80000008, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        sout[ ostream::HeadLevel::SYSTEM ].printk( WHITE, BLACK, "Physical Address size -> %08d | Linear Address size -> %08d\n", ( cpuFacName[ 0 ] & 0xff ), ( cpuFacName[ 0 ] >> 8 & 0xff ) );
        // 获得处理器支持的最大基础功能号
        this->cpuid( 0, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        sout[ ostream::HeadLevel::SYSTEM ].printk( WHITE, BLACK, "Max Basic Operation Code -> %#010x  | ", cpuFacName[ 0 ] );
        // 获得处理器支持的最大扩展功能号
        this->cpuid( 0x80000000, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        sout.printk( WHITE, BLACK, "Max Extended Operation Code -> %#010x\n", cpuFacName[ 0 ] );
    }
    auto CPUs::cpuid( IN Lib::Types::uint32_t mop, IN Lib::Types::uint32_t sop, IN Lib::Types::Ptr< Lib::Types::uint32_t > eax, IN Lib::Types::Ptr< Lib::Types::uint32_t > ebx, IN Lib::Types::Ptr< Lib::Types::uint32_t > ecx, IN Lib::Types::Ptr< Lib::Types::uint32_t > edx )->VOID {
        ASM( "cpuid\n\t"
             : "=a"( *eax ), "=b"( *ebx ), "=c"( *ecx ), "=d"( *edx )
             : "0"( mop ), "2"( sop ) );
        return;
    }
    auto CPUs::switch_to( IN OUT Lib::Types::Ptr< VOID > current, IN Lib::Types::Ptr< VOID > next )->VOID {
        using Kernel::Task;
        volatile Lib::Types::Ptr< Task::ThreadPCB > current_pcb { reinterpret_cast< Lib::Types::Ptr< Task::ThreadPCB > >( current ) }, next_pcb { reinterpret_cast< Lib::Types::Ptr< Task::ThreadPCB > >( next ) };
        current_pcb->cpu_frame = reinterpret_cast< Lib::Types::Ptr< Kernel::Task::ThreadFrame > >( reinterpret_cast< Lib::Types::uint64_t >( current_pcb->stack ) - sizeof *current_pcb + Kernel::TASK_STACK_SIZE - sizeof *current_pcb->cpu_frame );
        next_pcb->cpu_frame = reinterpret_cast< Lib::Types::Ptr< Kernel::Task::ThreadFrame > >( reinterpret_cast< Lib::Types::uint64_t >( next_pcb->stack ) - sizeof *next_pcb + Kernel::TASK_STACK_SIZE - sizeof *next_pcb->cpu_frame );
        ASM( "PUSHQ %1\n\t"
             "PUSHQ %0\n\t"
             "LEAQ _asm_thread_switch_to(%%RIP), %%RAX\n\t"
             "CALLQ *%%RAX\n\t" : : "g"( &current_pcb->cpu_frame ), "g"( &next_pcb->cpu_frame ) : );
    }
    auto CPUs::port_insw( IN Lib::Types::uint64_t port, IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::uint64_t nr )->VOID {
        ASM( "cld\n\t"
             "rep\n\t"
             "insw\n\t"
             "mfence\n\r" ::"d"( port ),
             "D"( buffer ), "c"( nr )
             : "memory" );
    }

    auto CPUs::port_outsw( IN Lib::Types::uint64_t port, IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::uint64_t nr )->VOID {
        ASM( "cld\n\t"
             "rep\n\t"
             "outsw\n\t"
             "mfence\n\r" ::"d"( port ),
             "S"( buffer ), "c"( nr )
             : "memory" );
    }

    auto CPUs::cli( VOID )->VOID {
        ASM( "cli\n\t" ::: "memory" );
    }

    auto CPUs::sti( VOID )->VOID {
        ASM( "sti\n\t" ::: "memory" );
    }

    auto CPUs::hlt( VOID )->VOID {
        ASM( "hlt\n\t" ::: "memory" );
    }

    auto CPUs::nop( VOID )->VOID {
        ASM( "nop\n\t" ::: "memory" );
    }

    auto CPUs::rdmsr( IN Lib::Types::uint64_t address )->Lib::Types::uint64_t {
        Lib::Types::uint32_t tmp0 { };
        Lib::Types::uint32_t tmp1 { };
        ASM( "rdmsr	\n\t"
             : "=d"( tmp0 ), "=a"( tmp1 )
             : "c"( address )
             : "memory" );
        return static_cast< Lib::Types::uint64_t >( tmp0 ) << 32 | tmp1;
    }

    auto CPUs::wrmsr( IN Lib::Types::uint64_t address,
                      IN Lib::Types::uint64_t value )
        ->VOID {
        ASM( "wrmsr	\n\t" ::"d"( value >> 32 ), "a"( value & 0xffffffff ), "c"( address )
             : "memory" );
    }

    auto CPUs::get_rsp( VOID )->Lib::Types::uint64_t {
        Lib::Types::uint64_t rsp { };
        ASM( "movq	%%rsp, %0	\n\t"
             : "=r"( rsp )::"memory" );
        return rsp;
    }

    auto CPUs::get_rflags( VOID )->Lib::Types::uint64_t {
        Lib::Types::uint64_t rsp_flags { };
        ASM( "pushfq	\n\t"
             "movq	(%%rsp), %0	\n\t"
             "popfq	\n\t"
             : "=r"( rsp_flags )
             :
             : "memory" );
        return rsp_flags;
    }
    auto CPUs::io_in8( IN Lib::Types::uint16_t port )->Lib::Types::uint8_t {
        Lib::Types::uint8_t ret { };
        ASM( "inb	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
    }
    auto CPUs::io_in16( IN Lib::Types::uint16_t port )->Lib::Types::uint16_t {
        Lib::Types::uint16_t ret { };
        ASM( "inw	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
    }

    auto CPUs::io_in32( IN Lib::Types::uint16_t port )->Lib::Types::uint32_t {
        Lib::Types::uint32_t ret { };
        ASM( "inl	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
    }

    auto CPUs::io_out8( IN Lib::Types::uint16_t port,
                        IN Lib::Types::uint8_t value )
        ->VOID {
        ASM( "outb %b[value],%w[port];"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
    }

    auto CPUs::io_out16( IN Lib::Types::uint16_t port,
                         IN Lib::Types::uint16_t value )
        ->VOID {
        ASM( "outw %w[value],%w[port];"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
    }

    auto CPUs::io_out32( IN Lib::Types::uint16_t port,
                         IN Lib::Types::uint32_t value )
        ->VOID {
        ASM( "outl %[value],%w[port];"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
    }

    auto CPUs::read_cr4( VOID )->Lib::Types::uint64_t {
        long cr4 { };
        ASM( "movq %%cr4, %0" : "=r"( cr4 )::"memory" );
        return Lib::Types::uint64_t( cr4 );
    }

    auto CPUs::write_cr4( IN Lib::Types::uint64_t cr4 )->VOID {
        ASM( "movq %0, %%cr4" ::"r"( long( cr4 ) ) : "memory" );
    }
    auto CPUs::read_cr3( VOID )->Lib::Types::uint64_t {
        long cr3 { };
        ASM( "movq %%cr3, %0" : "=r"( cr3 )::"memory" );
        return Lib::Types::uint64_t( cr3 );
    }

    auto CPUs::write_cr3( IN Lib::Types::uint64_t cr3 )->VOID {
        ASM( "movq %0, %%cr3" ::"r"( long( cr3 ) ) : "memory" );
    }

    auto CPUs::read_cr2( VOID )->Lib::Types::uint64_t {
        long cr2 { };
        ASM( "movq %%cr2, %0" : "=r"( cr2 )::"memory" );
        return Lib::Types::uint64_t( cr2 );
    }

    auto CPUs::write_cr2( IN Lib::Types::uint64_t cr2 )->VOID {
        ASM( "movq %0, %%cr2" ::"r"( long( cr2 ) ) : "memory" );
    }

    auto CPUs::read_cr1( VOID )->Lib::Types::uint64_t {
        long cr1 { };
        ASM( "movq %%cr1, %0" : "=r"( cr1 )::"memory" );
        return Lib::Types::uint64_t( cr1 );
    }

    auto CPUs::write_cr1( IN Lib::Types::uint64_t cr1 )->VOID {
        ASM( "movq %0, %%cr1" ::"r"( long( cr1 ) ) : "memory" );
    }

    auto CPUs::read_cr0( VOID )->Lib::Types::uint64_t {
        long cr0 { };
        ASM( "movq %%cr0, %0" : "=r"( cr0 )::"memory" );
        return Lib::Types::uint64_t( cr0 );
    }

    auto CPUs::write_cr0( IN Lib::Types::uint64_t cr0 )->VOID {
        ASM( "movq %0, %%cr0" ::"r"( long( cr0 ) ) : "memory" );
    }

    auto CPUs::invlpg( IN Lib::Types::Ptr< VOID > address )->VOID {
        ASM( "invlpg (%0)" : : "r"( address ) : "memory" );
    }
    auto CPUs::pause( VOID )->VOID {
        ASM( "pause" );
    }
    auto CPUs::mfence( VOID )->VOID {
        ASM( "mfence" ::: "memory" );
    }
    auto CPUs::lfence( VOID )->VOID {
        ASM( "lfence" ::: "memory" );
    }
    auto CPUs::sfence( VOID )->VOID {
        ASM( "sfence" ::: "memory" );
    }
    auto CPUs::set_page_table( IN Lib::Types::Ptr< Lib::Types::uint64_t > mmap )->VOID {
        ASM( "MOVQ %0, %%CR3\n\t" ::"r"( mmap ) );
    }
}