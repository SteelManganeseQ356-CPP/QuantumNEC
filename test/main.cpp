#include <iostream>
#include <functional>
constexpr const auto RFLAGS_MBS { 1 << 1 };
constexpr const auto RFLAGS_IF_1 { 1 << 9 };
constexpr const auto RFLAGS_IF_0 { 0 << 9 };
constexpr const auto RFLAGS_IOPL_0 { 1 << 12 };
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
    return 0;
}