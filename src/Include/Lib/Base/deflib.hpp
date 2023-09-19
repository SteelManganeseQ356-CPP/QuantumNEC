#pragma once
#include "attribute.hpp"
#include <Arch/ArchInfo.hpp>
#include <Lib/Types/type_base.hpp>
#define container_of( ptr, type, member )                                         \
    ( {                                                                           \
        decltype( ( (type *)0 )->member ) *p = ( ptr );                           \
        (type *)( (unsigned long)p - (unsigned long)&( ( (type *)0 )->member ) ); \
    } )

PUBLIC namespace QuantumNEC::Lib::Base {
    PUBLIC constexpr inline auto CHECKFLAG( const auto &field, const auto &flag ) {
        return field & flag;
    }
    PUBLIC constexpr const auto NR_CPUS { 8 };
    PUBLIC constexpr const auto RFLAGS_CF { ( 1 << 0 ) };
    PUBLIC constexpr const auto RFLAGS_PF { ( 1 << 2 ) };
    PUBLIC constexpr const auto RFLAGS_AF { ( 1 << 4 ) };
    PUBLIC constexpr const auto RFLAGS_ZF { ( 1 << 6 ) };
    PUBLIC constexpr const auto RFLAGS_SF { ( 1 << 7 ) };
    PUBLIC constexpr const auto RFLAGS_TF { ( 1 << 8 ) };
    PUBLIC constexpr const auto RFLAGS_IF { ( 1 << 9 ) };
    PUBLIC constexpr const auto RFLAGS_DF { ( 1 << 10 ) };
    PUBLIC constexpr const auto RFLAGS_OF { ( 1 << 11 ) };
    PUBLIC constexpr const auto RFLAGS_IOPL1 { ( 1 << 12 ) };
    PUBLIC constexpr const auto RFLAGS_IOPL2 { ( 1 << 13 ) };
    PUBLIC constexpr const auto RFLAGS_NT { ( 1 << 14 ) };
    PUBLIC constexpr const auto RFLAGS_RF { ( 1 << 16 ) };
    PUBLIC constexpr const auto RFLAGS_VM { ( 1 << 17 ) };
    PUBLIC constexpr const auto RFLAGS_AC { ( 1 << 18 ) };
    PUBLIC constexpr const auto RFLAGS_VIF { ( 1 << 19 ) };
    PUBLIC constexpr const auto RFLAGS_VIP { ( 1 << 20 ) };
    PUBLIC constexpr const auto RFLAGS_ID { ( 1 << 21 ) };
    PUBLIC constexpr const auto CRLF { "\r\n" };
    PUBLIC constexpr auto IS_DIGIT( const auto &c ) {
        return c >= '0' && c <= '9';
    };
    PUBLIC constexpr auto BIT( const auto &x ) {
        return (unsigned long long)1 << ( x );
    }
    PUBLIC constexpr const auto L1_CACHE_BYTES { 32 };
    PUBLIC constexpr auto DIV_ROUND_UP( const auto &X, const auto &STEP ) {
        return ( ( ( X ) + ( STEP - 1 ) ) / STEP );
    }
}     // namespace QuantumNEC::Lib::Base

#define CRLF_STR( s ) ( s CRLF )
#define STR_LENGTH( s ) s, sizeof( s ) - 1
#define ITER( n ) for ( QuantumNEC::Lib::Types::uint64_t i = 0; i < ( n ); ++i )
#define MATCH( x, __code__ ) \
    case x:                  \
        __code__;            \
        break;
#if SYSTEM_ARCH == x86_64
#define SWITCH_STACK( stackAddr ) ASM( "movq %0, %%rsp" ::"r"( stackAddr ) )
#define _BARRIER "mfence\n\t"
#define BARRIER ASM( _BARRIER );
#define DO_DIV( n, base )                             \
    ( {                                               \
        int __res;                                    \
        __asm__( "divq %%rcx"                         \
                 : "=a"( n ), "=d"( __res )           \
                 : "0"( n ), "1"( 0 ), "c"( base ) ); \
        __res;                                        \
    } )
#else
#endif
#define OFFSETOF( type, member ) __builtin_offsetof( type, member )
#define _C_LINK extern "C"
#define _CXX_LINK extern "C++"
#define SYMBOL_NAME( X ) X
#define SYMBOL_NAME_STR( X ) #X
#define SYMBOL_NAME_LABEL( X ) X##: