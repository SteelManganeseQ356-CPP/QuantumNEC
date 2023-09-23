#include <Lib/Base/bitmap.hpp>
#include <Lib/STL/string>

#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Lib::Base {
    BitmapManagement::BitmapManagement( Lib::Types::Ptr< BitMapData > btmp ) :
        bitmap { btmp } {
        Lib::STL::memset( this->bitmap->bits, 0, this->bitmap->mapLen );
    }
    BitmapManagement::~BitmapManagement( VOID ) {
    }
    auto BitmapManagement::scan( Lib::Types::size_t bit_idx )->Lib::Types::BOOL {
        return this->bitmap->bits[ bit_idx / 8 ] & ( BITMAP_MASK << ( bit_idx % 8 ) );
    }
    auto BitmapManagement::alloc( Lib::Types::size_t cnt )->Types::int64_t {
        Lib::Types::size_t index { };
        /* 寻找第一个空的bit所在位 */
        /* 位图已满,找不到空位 */
        while ( index < this->bitmap->mapLen && this->bitmap->bits[ index ] == 0xff ) {
            index++;
        }
        if ( index >= this->bitmap->mapLen || index == this->bitmap->mapLen ) {
            return -1; /* 分配失败 */
        }
        /* 发现有空位,*/
        Lib::Types::size_t bitIndex { };
        /* 位图中寻找空闲位的索引 */
        while ( ( ( Lib::Types::uint8_t )( BITMAP_MASK << bitIndex ) ) & this->bitmap->bits[ index ] ) {
            bitIndex++;
        }
        Lib::Types::int64_t bitIndexStart { static_cast< Lib::Types::int64_t >( index * 8 + bitIndex ) }; /* 空闲位在整个位图中的下标 */
        /* 只分配一个位, 那就是bitIndexStart位 */
        if ( cnt == 1 ) {
            return bitIndexStart;
        }
        Lib::Types::size_t bitRem { this->bitmap->mapLen * 8 - bitIndexStart }; /* 剩下的位 */
        Lib::Types::int64_t nextBitIndex { bitIndexStart + 1 };
        Lib::Types::size_t count { 1 }; /* 找到的空闲位的个数,上面已经找到一个了 */
        bitIndexStart = -1;             /* 找不到时直接返回-1 */
        while ( bitRem > 0 ) {
            if ( !( this->scan( nextBitIndex ) ) ) {
                /* 下一个bit位是0,那就又找到一个空bit位 */
                count++;
            }
            else {
                /* next_bit_index位不是0,重新找空闲位 */
                count = 0;
            }
            if ( count == cnt ) {
                /* 找到了连续cnt个空闲位 */
                bitIndexStart = nextBitIndex - cnt + 1; /* 空闲位的起始下标 */
                break;                                  /* 寻找结束,退出循环 */
            }
            bitRem--;
            nextBitIndex++;
        }
        return bitIndexStart;
    }
    auto BitmapManagement::set( Lib::Types::size_t bit_idx, Lib::Types::int8_t value )->VOID {
        /* 一般都会用个 0x1 这样的数对字节中的位操作 74 * 将 1
         * 任意移动后再取反，或者先取反再移位，可用来对位置 0 操作。*/
        switch ( value ) {
        case 0:     // 如果 value 为 0
            this->bitmap->bits[ bit_idx / 8 ] &= ( ~( BITMAP_MASK << bit_idx % 8 ) );
            break;
        case 1:     // 如果 value 为 1
            this->bitmap->bits[ bit_idx / 8 ] |= ( BITMAP_MASK << bit_idx % 8 );
            break;
        default:
            break;
        }
    }
}     // namespace QuantumNEC::Lib::Base