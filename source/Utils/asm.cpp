#include <Utils/asm.hpp>
#include <Arch/ArchInfo.hpp>
PUBLIC namespace QuantumNEC::Utils {
    PUBLIC auto port_insw( IN Lib::Types::uint64_t port, IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::uint64_t nr )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "cld\n\t"
             "rep\n\t"
             "insw\n\t"
             "mfence\n\r" ::"d"( port ),
             "D"( buffer ), "c"( nr )
             : "memory" );
#elif SYSTEM_ARCH == risc_v

#endif
    }
    PUBLIC auto port_outsw( IN Lib::Types::uint64_t port, IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::uint64_t nr )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "cld\n\t"
             "rep\n\t"
             "outsw\n\t"
             "mfence\n\r" ::"d"( port ),
             "S"( buffer ), "c"( nr )
             : "memory" );
#elif SYSTEM_ARCH == risc_v

#endif
    }
    PUBLIC auto cli( VOID )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "cli\n\t" ::: "memory" );
#endif
    }
    PUBLIC auto sti( VOID )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "sti\n\t" ::: "memory" );
#endif
    }
    PUBLIC auto hlt( VOID )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "hlt\n\t" ::: "memory" );
#endif
    }
    PUBLIC auto nop( VOID )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "nop\n\t" ::: "memory" );
#endif
    }
    PUBLIC auto rdmsr( IN Lib::Types::uint64_t address )->Lib::Types::uint64_t {
#if SYSTEM_ARCH == x86_64
        Lib::Types::uint32_t tmp0 { };
        Lib::Types::uint32_t tmp1 { };
        ASM( "rdmsr	\n\t"
             : "=d"( tmp0 ), "=a"( tmp1 )
             : "c"( address )
             : "memory" );
        return static_cast< Lib::Types::uint64_t >( tmp0 ) << 32 | tmp1;
#endif
    }

    PUBLIC auto wrmsr( IN Lib::Types::uint64_t address,
                       IN Lib::Types::uint64_t value )
        ->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "wrmsr	\n\t" ::"d"( value >> 32 ), "a"( value & 0xffffffff ), "c"( address )
             : "memory" );
#endif
    }

    PUBLIC auto getRsp( VOID )->Lib::Types::uint64_t {
#if SYSTEM_ARCH == x86_64
        Lib::Types::uint64_t rsp { };
        ASM( "movq	%%rsp, %0	\n\t"
             : "=r"( rsp )::"memory" );
        return rsp;
#endif
    }

    PUBLIC auto getRflags( VOID )->Lib::Types::uint64_t {
#if SYSTEM_ARCH == x86_64
        Lib::Types::uint64_t rspFlags { };
        ASM( "pushfq	\n\t"
             "movq	(%%rsp), %0	\n\t"
             "popfq	\n\t"
             : "=r"( rspFlags )
             :
             : "memory" );
        return rspFlags;
#endif
    }

    PUBLIC auto getEflags( VOID )->Lib::Types::uint64_t {
#if SYSTEM_ARCH == x86_64
        Lib::Types::uint64_t EFLAG_VAR { };
        ASM( "pushfq\n\t" /* 将flage寄存器压栈 */
             "popq %q[EFLAG_VAR]\n\t"
             : [EFLAG_VAR] "=a"( EFLAG_VAR )
             :
             : "memory" );
        return EFLAG_VAR;
#endif
    }
    PUBLIC auto io_in8( IN Lib::Types::uint16_t port )->Lib::Types::uint8_t {
#if SYSTEM_ARCH == x86_64
        Lib::Types::uint8_t ret { };
        ASM( "inb	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
#endif
    }
    PUBLIC auto io_in16( IN Lib::Types::uint16_t port )->Lib::Types::uint16_t {
#if SYSTEM_ARCH == x86_64
        Lib::Types::uint16_t ret { };
        ASM( "inw	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
#endif
    }
    PUBLIC auto io_in32( IN Lib::Types::uint16_t port )->Lib::Types::uint32_t {
#if SYSTEM_ARCH == x86_64
        Lib::Types::uint32_t ret { };
        ASM( "inl	%%dx,	%0	\n\t"
             "mfence			\n\t"
             : "=a"( ret )
             : "d"( port )
             : "memory" );
        return ret;
#endif
    }
    PUBLIC auto io_out8( IN Lib::Types::uint16_t port,
                         IN Lib::Types::uint8_t value )
        ->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "outb %b[value],%w[port];"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
#endif
    }
    PUBLIC auto io_out16( IN Lib::Types::uint16_t port,
                          IN Lib::Types::uint16_t value )
        ->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "outw %w[value],%w[port];"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
#endif
    }
    PUBLIC auto io_out32( IN Lib::Types::uint16_t port,
                          IN Lib::Types::uint32_t value )
        ->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "outl %0,%1"
             :
             : [value] "a"( value ), [port] "d"( port )
             : "memory" );
#endif
    }
    PUBLIC auto readCr4( VOID )->Lib::Types::uint64_t {
        long cr4 { };
#if SYSTEM_ARCH == x86_64
        ASM( "movq %%cr4, %0" : "=r"( cr4 )::"memory" );
#endif
        return Lib::Types::uint64_t( cr4 );
    }
    PUBLIC auto writeCr4( IN Lib::Types::uint64_t cr4 )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "movq %0, %%cr4" ::"r"( long( cr4 ) ) : "memory" );
#endif
    }
    PUBLIC auto readCr3( VOID )->Lib::Types::uint64_t {
        long cr3 { };
#if SYSTEM_ARCH == x86_64
        ASM( "movq %%cr3, %0" : "=r"( cr3 )::"memory" );
#endif
        return Lib::Types::uint64_t( cr3 );
    }
    PUBLIC auto writeCr3( IN Lib::Types::uint64_t cr3 )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "movq %0, %%cr3" ::"r"( long( cr3 ) ) : "memory" );
#endif
    }
    PUBLIC auto readCr2( VOID )->Lib::Types::uint64_t {
        long cr2 { };
#if SYSTEM_ARCH == x86_64
        ASM( "movq %%cr2, %0" : "=r"( cr2 )::"memory" );
#endif
        return Lib::Types::uint64_t( cr2 );
    }
    PUBLIC auto writeCr2( IN Lib::Types::uint64_t cr2 )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "movq %0, %%cr2" ::"r"( long( cr2 ) ) : "memory" );
#endif
    }
    PUBLIC auto readCr1( VOID )->Lib::Types::uint64_t {
        long cr1 { };
#if SYSTEM_ARCH == x86_64
        ASM( "movq %%cr1, %0" : "=r"( cr1 )::"memory" );
#endif
        return Lib::Types::uint64_t( cr1 );
    }
    PUBLIC auto writeCr1( IN Lib::Types::uint64_t cr1 )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "movq %0, %%cr1" ::"r"( long( cr1 ) ) : "memory" );
#endif
    }
    PUBLIC auto readCr0( VOID )->Lib::Types::uint64_t {
        long cr0 { };
#if SYSTEM_ARCH == x86_64
        ASM( "movq %%cr0, %0" : "=r"( cr0 )::"memory" );
#endif
        return Lib::Types::uint64_t( cr0 );
    }
    PUBLIC auto writeCr0( IN Lib::Types::uint64_t cr0 )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "movq %0, %%cr0" ::"r"( long( cr0 ) ) : "memory" );
#endif
    }
    PUBLIC auto invlpg( IN Lib::Types::Ptr< VOID > address )->VOID {
#if SYSTEM_ARCH == x86_64
        ASM( "invlpg (%0)" : : "r"( address ) : "memory" );
#endif
    }
}