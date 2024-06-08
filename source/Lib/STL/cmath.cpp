#include <Lib/STL/cmath>
#include <Arch/ArchInfo.hpp>
#include <Lib/Base/deflib.hpp>
PUBLIC namespace {
    using namespace QuantumNEC::Lib::Types;
    PRIVATE constexpr auto __Accuracy { 0.0000001f };
}     // namespace
PUBLIC namespace QuantumNEC::Lib::STL {
    PUBLIC auto floor( IN Lib::Types::double32_t x )->Lib::Types::double32_t {
        return ( x - 1 + __Accuracy ) / 1;
    }

    PUBLIC auto ceil( IN Lib::Types::double32_t x )->Lib::Types::double32_t {
        return ( x + 1 - __Accuracy ) / 1;
    }

    PUBLIC auto round( IN Lib::Types::double32_t x )->Lib::Types::double32_t {
        return ( x > 0 ) ? floor( x + 0.5 ) : ceil( x - 0.5 );
    }

    PUBLIC auto sqrt( IN Lib::Types::double32_t x )->Lib::Types::double32_t {
        Lib::Types::double32_t result { };
        ASM( "fsqrt"
             : "=t"( result )
             : "0"( x ) );
        return result;
    }

    PUBLIC auto fmod( IN Lib::Types::double32_t x, Lib::Types::double32_t y )
        ->Lib::Types::double32_t {
        return x - static_cast< Lib::Types::int32_t >( x / y ) * y;
    }
    PUBLIC auto atan( IN Lib::Types::double32_t x )->Lib::Types::double32_t {
        STATIC CONST Lib::Types::uint32_t sign_mask { 0x80000000 };
        STATIC CONST Lib::Types::double32_t b { 0.596227f };
        Lib::Types::uint32_t ux_s { sign_mask & (Lib::Types::uint32_t &)x };
        Lib::Types::double32_t bx_a { abs( b * x ) };
        Lib::Types::double32_t num { bx_a + x * x };
        Lib::Types::double32_t atan_1q { num / ( 1.f + bx_a + num ) };
        Lib::Types::uint32_t atan_2q { ux_s | (Lib::Types::uint32_t &)atan_1q };
        return (Lib::Types::double32_t &)( atan_2q );
    }

    PUBLIC auto atan2( IN Lib::Types::double32_t x, Lib::Types::double32_t y )
        ->Lib::Types::double32_t {
        STATIC CONST Lib::Types::uint32_t sign_mask { 0x80000000 };
        STATIC CONST Lib::Types::double32_t b { 0.596227f };
        Lib::Types::uint32_t ux_s { sign_mask & (Lib::Types::uint32_t &)x };
        Lib::Types::uint32_t uy_s { sign_mask & (Lib::Types::uint32_t &)y };
        Lib::Types::double32_t q { ( Lib::Types::double32_t )( ( ~ux_s & uy_s ) >> 29
                                                               | ux_s >> 30 ) };
        Lib::Types::double32_t bxy_a { abs( b * x * y ) };
        Lib::Types::double32_t num { bxy_a + y * y };
        Lib::Types::double32_t atan_1q { num / ( x * x + bxy_a + num ) };
        Lib::Types::uint32_t uatan_2q { ( ux_s ^ uy_s )
                                        | (Lib::Types::uint32_t &)atan_1q };
        return q + (Lib::Types::double32_t &)( uatan_2q );
    }

    auto cos( Lib::Types::double32_t x )->Lib::Types::double32_t {
        Lib::Types::double32_t result { };
#if SYSTEM_ARCH == x86_64
        ASM( "fcos"
             : "=t"( result )
             : "0"( x ) );
#endif
        return result;
    }
    auto acos( Lib::Types::double32_t x )->Lib::Types::double32_t {
        return atan2( sqrt( 1.0 - x * x ), x );
    }
    auto sin( Lib::Types::double32_t x )->Lib::Types::double32_t {
        Lib::Types::double32_t result { };
#if SYSTEM_ARCH == x86_64
        ASM( "fsin"
             : "=t"( result )
             : "0"( x ) );
#endif
        return result;
    }
}