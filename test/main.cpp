#include <iostream>
#include <functional>
constexpr const auto RFLAGS_MBS { 1 << 1 };
constexpr const auto RFLAGS_IF_1 { 1 << 9 };
constexpr const auto RFLAGS_IF_0 { 0 << 9 };
constexpr const auto RFLAGS_IOPL_0 { 1 << 12 };
struct A
{
    unsigned long long a : 1;
    unsigned long long b : 1;
    unsigned long long c : 1;
    unsigned long long d : 1;
    unsigned long long e : 59;
};
auto main( void ) -> int {
    std::printf( "%lx\n\r", 1 << 4 );                                       // 1
    std::printf( "%lx\n\r", 1 << 1 );                                       // 2
    std::printf( "%lx\n\r", 1 << 9 );                                       // 4
    std::printf( "%lx\n\r", 0 << 9 );                                       // 8
    std::printf( "%lx\n\r", 0 << 12 );                                      // 10
    std::printf( "%lx\n\r", RFLAGS_IOPL_0 | RFLAGS_MBS | RFLAGS_IF_1 );     // 10
    std::function< void( void ) > f = [ & ]( ) {
        return;
    };
    A sb { };
    sb.a = 1;
    sb.b = 1;
    sb.c = 1;
    sb.d = 1;
    sb.e = 0xffffffffffffffff;
    long address;
    address = ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) | 0xffffffffffffffff;
    //  0001
    //  0010
    //  0100
    //  1000
    std::printf( "%x%x%x%x\n", sb.a, sb.b, sb.c, sb.d );
    std::printf( "%lx\n", address );
    return 0;
}