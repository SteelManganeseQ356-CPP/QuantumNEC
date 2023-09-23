#pragma once
#include <Arch/ArchInfo.hpp>
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_base.hpp>
#include <Lib/Types/type_int.hpp>
/**
 * @brief 定义
 */
#define ASM __asm__ __volatile__
PUBLIC namespace QuantumNEC::Utils {
    PUBLIC inline auto port_insw( auto port, auto buffer, auto nr )->VOID {
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

    PUBLIC inline auto port_outsw( auto port, auto buffer, auto nr )->VOID {
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
    /**
     * @brief
     */
    PUBLIC auto cli( VOID )->VOID;
    /**
     * @brief
     */
    PUBLIC auto sti( VOID )->VOID;
    /**
     * @brief
     */
    PUBLIC auto hlt( VOID )->VOID;
    /**
     * @brief
     */
    PUBLIC auto nop( VOID )->VOID;
    /**
     * @brief
     */
    PUBLIC auto rdmsr( IN Lib::Types::uint64_t address )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto wrmsr( IN Lib::Types::uint64_t address, IN Lib::Types::uint64_t value )->VOID;
    /**
     * @brief
     */
    PUBLIC auto getRsp( VOID )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto getRflags( VOID )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto getEflags( VOID )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto io_in8( IN Lib::Types::uint16_t port )->Lib::Types::uint8_t;
    /**
     * @brief
     */
    PUBLIC auto io_in16( IN Lib::Types::uint16_t port )->Lib::Types::uint16_t;
    /**
     * @brief
     */
    PUBLIC auto io_in32( IN Lib::Types::uint16_t port )->Lib::Types::uint32_t;
    /**
     * @brief
     */
    PUBLIC auto io_out8( IN Lib::Types::uint16_t port, IN Lib::Types::uint8_t value )->VOID;
    /**
     * @brief
     */
    PUBLIC auto io_out16( IN Lib::Types::uint16_t port, IN Lib::Types::uint16_t value )->VOID;
    /**
     * @brief
     */
    PUBLIC auto io_out32( IN Lib::Types::uint16_t port, IN Lib::Types::uint32_t value )->VOID;
    /**
     * @brief
     */
    PUBLIC auto readCr4( VOID )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto writeCr4( IN Lib::Types::uint64_t cr3 )->VOID;
    /**
     * @brief
     */
    PUBLIC auto readCr3( VOID )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto writeCr3( IN Lib::Types::uint64_t cr3 )->VOID;
    /**
     * @brief
     */
    PUBLIC auto readCr2( VOID )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto writeCr2( IN Lib::Types::uint64_t cr2 )->VOID;
    /**
     * @brief
     */
    PUBLIC auto readCr1( VOID )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto writeCr1( IN Lib::Types::uint64_t cr0 )->VOID;
    /**
     * @brief
     */
    PUBLIC auto readCr0( VOID )->Lib::Types::uint64_t;
    /**
     * @brief
     */
    PUBLIC auto writeCr0( IN Lib::Types::uint64_t cr0 )->VOID;
}