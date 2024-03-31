#include <Lib/STL/cstring>
PUBLIC namespace QuantumNEC::Lib::STL {
    PUBLIC auto strcpy( IN OUT Lib::Types::Ptr< Lib::Types::char_t > Dest,
                        IN Lib::Types::Ptr< CONST Lib::Types::char_t > Src )
        ->Lib::Types::Ptr< Lib::Types::char_t > {
        Lib::Types::Ptr< Lib::Types::char_t > tmp { Dest };
        for ( Lib::Types::size_t i { }; Src[ i ]; ++i )
            Dest[ i ] = Src[ i ];
        return tmp;
    }
    PUBLIC auto strncpy( IN OUT Lib::Types::Ptr< Lib::Types::char_t > Dest,
                         IN Lib::Types::Ptr< CONST Lib::Types::char_t > Src,
                         IN CONST Lib::Types::int64_t Count )
        ->Lib::Types::Ptr< Lib::Types::char_t > {
        Lib::Types::Ptr< Lib::Types::char_t > tmp { Dest };
        Types::int64_t n { Count };
        for ( Lib::Types::size_t i { }; n-- > 0 && Src[ i ]; ++i )
            Dest[ i ] = Src[ i ];
        return tmp;
    }

    PUBLIC auto strcat( IN OUT Lib::Types::Ptr< Lib::Types::char_t > Dest,
                        IN Lib::Types::Ptr< CONST Lib::Types::char_t > Src )
        ->Lib::Types::Ptr< Lib::Types::char_t > {
        Lib::Types::Ptr< Lib::Types::char_t > tmp { Dest };
        while ( *Dest )
            Dest++;
        for ( Lib::Types::size_t i { }; Src[ i ]; ++i )
            Dest[ i ] = Src[ i ];
        return tmp;
    }
    PUBLIC auto strcat( IN OUT Lib::Types::Ptr< Lib::Types::char_t > Dest,
                        IN Lib::Types::Ptr< CONST Lib::Types::char_t > Src,
                        IN Lib::Types::size_t Count )
        ->Lib::Types::Ptr< Lib::Types::char_t > {
        Lib::Types::Ptr< Lib::Types::char_t > tmp { Dest };
        while ( *Dest )
            Dest++;
        for ( Lib::Types::size_t i { }; Count-- > 0 && Src[ i ]; ++i )
            Dest[ i ] = Src[ i ];
        return tmp;
    }
    PUBLIC Lib::Types::int32_t strcmp(
        IN Lib::Types::Ptr< CONST Lib::Types::char_t > FirstPart,
        IN Lib::Types::Ptr< CONST Lib::Types::char_t > SecondPart ) {
        while ( *FirstPart == *SecondPart ) {
            if ( !( *FirstPart ) )
                return 0;
            FirstPart++;
            SecondPart++;
        }
        return *FirstPart - *SecondPart;
    }

    PUBLIC Lib::Types::int32_t strncmp(
        IN Lib::Types::Ptr< CONST Lib::Types::char_t > FirstPart,
        IN Lib::Types::Ptr< CONST Lib::Types::char_t > SecondPart,
        IN CONST Lib::Types::int64_t Count ) {
        auto count { Count };
        if ( !count )
            return 0;
        while ( --count && *FirstPart
                && *FirstPart == *SecondPart )     // 把第count个字符留到最后处理。
        {
            FirstPart++;
            SecondPart++;
        }
        return *reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >(
                   const_cast< Lib::Types::Ptr< Lib::Types::char_t > >( FirstPart ) )
               - *reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >(
                   const_cast< Lib::Types::Ptr< Lib::Types::char_t > >( SecondPart ) );
    }

    PUBLIC auto strlen( IN Lib::Types::Ptr< CONST Lib::Types::char_t > String )
        ->Lib::Types::size_t {
        auto strSize { 0ULL };
        auto tmp { String };
        while ( *tmp ) {
            ++tmp;
            ++strSize;
        }
        return strSize;
    }
    PUBLIC auto strchr( IN Lib::Types::Ptr< CONST Lib::Types::char_t > dest, Lib::Types::int32_t ch )->Lib::Types::Ptr< Lib::Types::char_t > {
        if ( !dest ) {
            return NULL;
        }
        Lib::Types::Ptr< Lib::Types::char_t > src { const_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest ) };
        while ( *src++ ) {
            if ( *src == ch ) {
                return src;
            }
        }
        return NULL;
    }
    PUBLIC auto strrchr( IN Lib::Types::Ptr< CONST Lib::Types::char_t > dest, Lib::Types::int32_t ch )->Lib::Types::Ptr< Lib::Types::char_t > {
        Lib::Types::Ptr< Lib::Types::char_t > _src_one { strchr( dest, ch ) };
        Lib::Types::Ptr< Lib::Types::char_t > _src_two { };
        while ( _src_one ) {
            _src_two = strchr( _src_one + 1, ch );
            if ( _src_two ) {
                _src_one = _src_two;
            }
            else
                return _src_one;     // 当这个没有匹配到时即返回上一步的位置
        }
        return NULL;     // 全部未匹配
    }
    PUBLIC auto memset( IN OUT Lib::Types::Ptr< VOID > dest,
                        IN CONST Lib::Types::uint8_t value,
                        IN CONST Lib::Types::uint64_t size )
        ->Lib::Types::Ptr< VOID > {
        auto pdest { reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >(
            dest ) };
        auto n { size };
        while ( n-- > 0 )
            *pdest++ = value;
        return dest;
    }

    PUBLIC auto memcpy( IN OUT Lib::Types::Ptr< VOID > dest,
                        IN Lib::Types::Ptr< CONST VOID > src,
                        IN Lib::Types::uint64_t size )
        ->Lib::Types::Ptr< VOID > {
        Lib::Types::Ptr< VOID > ret { dest };
        /*
         * 从较低地址复制到较高地址
         */
        while ( size-- ) {
            *reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest ) = *reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( const_cast< Lib::Types::Ptr< VOID > >( src ) );
            dest = reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest )
                   + 1;
            src = reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( const_cast< Lib::Types::Ptr< VOID > >( src ) ) + 1;
        }
        return reinterpret_cast< Lib::Types::Ptr< VOID > >( ret );
    }
    PUBLIC auto memcmp( IN CONST Lib::Types::Ptr< VOID > buf1,
                        IN CONST Lib::Types::Ptr< VOID > buf2,
                        IN CONST Lib::Types::uint64_t size )
        ->Lib::Types::int32_t {
        auto pdest1 { reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >(
            const_cast< Lib::Types::Ptr< VOID > >( buf1 ) ) },
            pdest2 { reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >(
                const_cast< Lib::Types::Ptr< VOID > >( buf2 ) ) };
        auto n { size };
        while ( n-- > 0 ) {
            if ( *pdest1 != *pdest2 )
                return *pdest1 - *pdest2;
            pdest1++, pdest2++;
        }
        return 0;
    }
    PUBLIC auto memmove( IN OUT Lib::Types::Ptr< VOID > dest,
                         IN OUT Lib::Types::Ptr< CONST VOID > src,
                         IN Lib::Types::size_t size )
        ->Lib::Types::Ptr< VOID > {
        Lib::Types::Ptr< VOID > __src { const_cast< Lib::Types::Ptr< VOID > >( src ) };
        Lib::Types::Ptr< VOID > ret { dest };
        if ( dest <= src || reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest ) >= ( reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( __src ) + size ) ) {
            /* 非重叠缓冲区
             * 从较低的地址复制到较高的地址 */
            while ( size-- ) {
                *reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest ) = *reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( __src );
                dest = reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest ) + 1;
                __src = reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( __src ) + 1;
            }
        }
        else {
            /* 重叠缓冲区
             * 从较高的地址复制到较低的地址 */
            dest = reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest ) + size - 1;
            __src = reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( __src ) + size - 1;
            while ( size-- ) {
                *reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest ) = *reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( __src );
                dest = reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( dest ) - 1;
                __src = reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( __src ) - 1;
            }
        }
        return ret;
    }
    PUBLIC auto memchr( IN OUT Lib::Types::Ptr< VOID > buf,
                        IN Lib::Types::int8_t value, IN Lib::Types::size_t size )
        ->Lib::Types::Ptr< VOID > {
        if ( !buf )
            return NULL;
        auto p { reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( buf ) };
        while ( size-- ) {
            if ( *p != value ) {
                p++;
            }
            else
                return reinterpret_cast< Lib::Types::Ptr< VOID > >( p );
        }
        return NULL;
    }
    PUBLIC auto strstr( IN Lib::Types::Ptr< Lib::Types::char_t > haystack,
                        IN CONST Lib::Types::Ptr< Lib::Types::char_t > needle )
        ->Lib::Types::Ptr< Lib::Types::char_t > {
        if ( *needle ) {
            while ( *haystack ) {
                for ( Lib::Types::int32_t n { }; *( haystack + n ) == *( needle + n );
                      n++ ) {
                    if ( !*( needle + n + 1 ) )
                        return (char *)haystack;
                }
                haystack++;
            }
            return NULL;
        }
        else
            return haystack;
    }
    auto itoa( IN Lib::Types::int64_t num, IN Lib::Types::Ptr< Lib::Types::char_t > str, IN Lib::Types::uint32_t base )->VOID {
        STATIC Lib::Types::char_t digits[ 37 ] { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
        Types::int32_t i { };                    /* 作为下标来索引 */
        Lib::Types::int64_t is_negative { num }; /* 是否是负的？ */
        if ( num < 0 )                           /* 如果是负数 */
        {
            num = -num; /* 先转换为正数 */
        }
        /* 转换为字符串,不过是倒过来的 */
        do {
            str[ i ] = digits[ num % base ];
            i++;
            num = num / base;
        } while ( num > 0 );
        /* 是负数,就加上负号 */
        if ( is_negative < 0 ) {
            str[ i ] = '-';
            i++;
        }
        str[ i ] = '\0'; /* 加上字符串结尾 */
        Lib::Types::Ptr< Lib::Types::char_t > p { str };
        Lib::Types::Ptr< Lib::Types::char_t > q { str };
        Lib::Types::char_t tmp { };
        while ( *q != '\0' ) {
            q++;
        }
        q--;
        /* 把字符串倒过来 */
        while ( q > p ) {
            tmp = *p;
            *p = *q;
            p++;
            *q = tmp;
            q--;
        }
    }

    auto utoa( IN Lib::Types::uint64_t num, IN Lib::Types::Ptr< Lib::Types::char_t > str, IN Lib::Types::uint32_t base )->VOID {
        STATIC Lib::Types::char_t digits[ 37 ] { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
        Types::int32_t i { }; /* 作为下标来索引 */
        /* 转换为字符串,不过是倒过来的 */
        do {
            str[ i ] = digits[ num % base ];
            i++;
            num = num / base;
        } while ( num > 0 );
        str[ i ] = '\0'; /* 加上字符串结尾 */
        Lib::Types::Ptr< Lib::Types::char_t > p { str };
        Lib::Types::Ptr< Lib::Types::char_t > q { str };
        Lib::Types::char_t tmp { };
        while ( *q != '\0' ) {
            q++;
        }
        q--;
        /* 把字符串倒过来 */
        while ( q > p ) {
            tmp = *p;
            *p = *q;
            p++;
            *q = tmp;
            q--;
        }
        return;
    }
}
