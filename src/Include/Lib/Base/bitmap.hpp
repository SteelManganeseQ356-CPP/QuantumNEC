#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
#define BITMAP_MASK 1
PUBLIC namespace QuantumNEC::Lib::Base {
    PUBLIC typedef struct
    {
        Lib::Types::uint64_t mapLen;                     // 位图以字节为单位的长度
        Lib::Types::Ptr< Lib::Types::uint8_t > bits;     // 位图
    } BitMapData;

    PUBLIC class BitmapManagement
    {
    public:
        /**
         * @brief 初始化位图
         * @param btmp 要初始化的位图指针
         */
        explicit( TRUE ) BitmapManagement( IN Lib::Types::Ptr< BitMapData > btmp ) noexcept;
        virtual ~BitmapManagement( VOID ) noexcept;

    public:
        /**
         * @brief 判断位图中bit_idx是否为1
         * @param btmp 位图指针
         * @param bit_index bit位下标
         * @return 为1返回true 为0返回false
         */
        auto scan( IN Lib::Types::size_t bit_idx ) -> Lib::Types::BOOL;
        /**
         * @brief 在位图中分配位
         * @param btmp 位图指针
         * @param cnt 要分配的位数
         * @retval 分配成功返回位的下标 分配失败返回-1
         */
        auto alloc( IN Lib::Types::size_t cnt ) -> Lib::Types::int64_t;
        /**
         * @brief 将位图的bit_index位设为value
         * @param btmp 位图指针
         * @param bit_index 要设置的位的下标
         * @param value 设置值
         */
        auto set( IN Lib::Types::size_t bit_idx, IN Lib::Types::int8_t value ) -> VOID;

    public:
        /**
         * @brief 更换bitmap
         * @param btmp 要更换的对象
         */
        auto
        changeBitmap( IN Lib::Types::Ptr< BitMapData > btmp ) -> VOID {
            this->bitmap = btmp;
        }

    private:
        Lib::Types::Ptr< BitMapData > bitmap;
    };
}