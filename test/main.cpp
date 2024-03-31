#include <iostream>
auto main( void ) -> int {
    std::printf( "%lx\n\r", 1 << 0 );                // 1
    std::printf( "%lx\n\r", 1 << 1 );                // 2
    std::printf( "%lx\n\r", 1 << 2 );                // 4
    std::printf( "%lx\n\r", 1 << 3 );                // 8
    std::printf( "%lx\n\r", 1 << 4 );                // 10
    std::printf( "%lx\n\r", 0x80000 % 0x20000 );     // 10
    int *b = new int[ 4 ] { 1, 2, 3, 4 };

    std::cout << *(int *)( long( b ) + 4 ) << '\n';
    return 0;
}