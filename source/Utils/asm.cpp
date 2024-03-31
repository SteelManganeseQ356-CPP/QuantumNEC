#include <Utils/asm.hpp>
#include <Arch/ArchInfo.hpp>
PUBLIC namespace QuantumNEC::Utils {
    PUBLIC auto port_insw( IN Lib::Types::uint64_t port, IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::uint64_t nr )->VOID {
#if defined( __x86_64__ )
        ASM( "cld\n\t"
             "rep\n\t"
             "insw\n\t"
             "mfence\n\r" ::"d"( port ),
             "D"( buffer ), "c"( nr )
             : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto port_outsw( IN Lib::Types::uint64_t port, IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::uint64_t nr )->VOID {
#if defined( __x86_64__ )
        ASM( "cld\n\t"
             "rep\n\t"
             "outsw\n\t"
             "mfence\n\r" ::"d"( port ),
             "S"( buffer ), "c"( nr )
             : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto cli( VOID )->VOID {
#if defined( __x86_64__ )
        ASM( "cli\n\t" ::: "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto sti( VOID )->VOID {
#if defined( __x86_64__ )
        ASM( "sti\n\t" ::: "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto hlt( VOID )->VOID {
#if defined( __x86_64__ )
        ASM( "hlt\n\t" ::: "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto nop( VOID )->VOID {
#if defined( __x86_64__ )
        ASM( "nop\n\t" ::: "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto rdmsr( IN Lib::Types::uint64_t address )->Lib::Types::uint64_t {
#if defined( __x86_64__ )
        Lib::Types::uint32_t tmp0 { };
        Lib::Types::uint32_t tmp1 { };
        ASM( "rdmsr	\n\t"
             : "=d"( tmp0 ), "=a"( tmp1 )
             : "c"( address )
             : "memory" );
        return static_cast< Lib::Types::uint64_t >( tmp0 ) << 32 | tmp1;
#elif defined( __aarch64__ )
#endif
    }

    PUBLIC auto wrmsr( IN Lib::Types::uint64_t address,
                       IN Lib::Types::uint64_t value )
        ->VOID {
#if defined( __x86_64__ )
        ASM( "wrmsr	\n\t" ::"d"( value >> 32 ), "a"( value & 0xffffffff ), "c"( address )
             : "memory" );
#elif defined( __aarch64__ )
#endif
    }

    PUBLIC auto get_rsp( VOID )->Lib::Types::uint64_t {
#if defined( __x86_64__ )
        Lib::Types::uint64_t rsp { };
        ASM( "movq	%%rsp, %0	\n\t"
             : "=r"( rsp )::"memory" );
        return rsp;
#elif defined( __aarch64__ )
#endif
    }

    PUBLIC auto get_rflags( VOID )->Lib::Types::uint64_t {
#if defined( __x86_64__ )
        Lib::Types::uint64_t rspFlags { };
        ASM( "pushfq	\n\t"
             "movq	(%%rsp), %0	\n\t"
             "popfq	\n\t"
             : "=r"( rspFlags )
             :
             : "memory" );
        return rspFlags;
#elif defined( __aarch64__ )
#endif
    }

    PUBLIC auto get_eflags( VOID )->Lib::Types::uint64_t {
#if defined( __x86_64__ )
        Lib::Types::uint64_t EFLAG_VAR { };
        ASM( "pushfq\n\t" /* 将flage寄存器压栈 */
             "popq %q[EFLAG_VAR]\n\t"
             : [EFLAG_VAR] "=a"( EFLAG_VAR )
             :
             : "memory" );
        return EFLAG_VAR;
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto io_in8( IN Lib::Types::uint16_t port )->Lib::Types::uint8_t {
#if defined( __x86_64__ )
        Lib::Types::uint8_t ret { };
        ASM( "inb	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto io_in16( IN Lib::Types::uint16_t port )->Lib::Types::uint16_t {
#if defined( __x86_64__ )
        Lib::Types::uint16_t ret { };
        ASM( "inw	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto io_in32( IN Lib::Types::uint16_t port )->Lib::Types::uint32_t {
#if defined( __x86_64__ )
        Lib::Types::uint32_t ret { };
        ASM( "inl	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto io_out8( IN Lib::Types::uint16_t port,
                         IN Lib::Types::uint8_t value )
        ->VOID {
#if defined( __x86_64__ )
        ASM( "outb %b[value],%w[port];"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto io_out16( IN Lib::Types::uint16_t port,
                          IN Lib::Types::uint16_t value )
        ->VOID {
#if defined( __x86_64__ )
        ASM( "outw %w[value],%w[port];"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto io_out32( IN Lib::Types::uint16_t port,
                          IN Lib::Types::uint32_t value )
        ->VOID {
#if defined( __x86_64__ )
        ASM( "outl %0,%1"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto read_cr4( VOID )->Lib::Types::uint64_t {
        long cr4 { };
#if defined( __x86_64__ )
        ASM( "movq %%cr4, %0" : "=r"( cr4 )::"memory" );
#elif defined( __aarch64__ )
#endif
        return Lib::Types::uint64_t( cr4 );
    }
    PUBLIC auto write_cr4( IN Lib::Types::uint64_t cr4 )->VOID {
#if defined( __x86_64__ )
        ASM( "movq %0, %%cr4" ::"r"( long( cr4 ) ) : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto read_cr3( VOID )->Lib::Types::uint64_t {
        long cr3 { };
#if defined( __x86_64__ )
        ASM( "movq %%cr3, %0" : "=r"( cr3 )::"memory" );
#elif defined( __aarch64__ )
#endif
        return Lib::Types::uint64_t( cr3 );
    }
    PUBLIC auto write_cr3( IN Lib::Types::uint64_t cr3 )->VOID {
#if defined( __x86_64__ )
        ASM( "movq %0, %%cr3" ::"r"( long( cr3 ) ) : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto read_cr2( VOID )->Lib::Types::uint64_t {
        long cr2 { };
#if defined( __x86_64__ )
        ASM( "movq %%cr2, %0" : "=r"( cr2 )::"memory" );
#elif defined( __aarch64__ )
#endif
        return Lib::Types::uint64_t( cr2 );
    }
    PUBLIC auto write_cr2( IN Lib::Types::uint64_t cr2 )->VOID {
#if defined( __x86_64__ )
        ASM( "movq %0, %%cr2" ::"r"( long( cr2 ) ) : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto read_cr1( VOID )->Lib::Types::uint64_t {
        long cr1 { };
#if defined( __x86_64__ )
        ASM( "movq %%cr1, %0" : "=r"( cr1 )::"memory" );
#elif defined( __aarch64__ )
#endif
        return Lib::Types::uint64_t( cr1 );
    }
    PUBLIC auto write_cr1( IN Lib::Types::uint64_t cr1 )->VOID {
#if defined( __x86_64__ )
        ASM( "movq %0, %%cr1" ::"r"( long( cr1 ) ) : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto read_cr0( VOID )->Lib::Types::uint64_t {
        long cr0 { };
#if defined( __x86_64__ )
        ASM( "movq %%cr0, %0" : "=r"( cr0 )::"memory" );
#elif defined( __aarch64__ )
#endif
        return Lib::Types::uint64_t( cr0 );
    }
    PUBLIC auto write_cr0( IN Lib::Types::uint64_t cr0 )->VOID {
#if defined( __x86_64__ )
        ASM( "movq %0, %%cr0" ::"r"( long( cr0 ) ) : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto invlpg( IN Lib::Types::Ptr< VOID > address )->VOID {
#if defined( __x86_64__ )
        ASM( "invlpg (%0)" : : "r"( address ) : "memory" );
#elif defined( __aarch64__ )
#endif
    }
    PUBLIC auto pause( VOID )->VOID {
#if defined( __x86_64__ )
        ASM( "pause" );
#elif defined( __aarch64__ )
#endif
    }
}