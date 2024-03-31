#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Lib/Base/deflib.hpp>
#include <Lib/Types/Uefi.hpp>
#include <concepts>
PUBLIC namespace QuantumNEC::Architecture::Platform {
    /**
     * @brief 描述符管理模板
     * @tparam D 要管理的描述符，约定必须有一个set成员函数，接受一个D*参数，返回D&
     */
    PUBLIC template < typename D >
        requires requires( Lib::Types::Ptr< D > test ) {
            { test->set( test ) } -> std::same_as< D & >;
        }
    class DescriptorManagement
    {
    public:
        explicit( TRUE ) DescriptorManagement( IN Lib::Types::Ptr< D > _descriptor, IN Lib::Types::uint16_t _descriptorCount ) noexcept( TRUE ) :
            xdtr { Lib::Types::uint16_t( sizeof( D ) * _descriptorCount - 1 ), _descriptor } {
        }
        virtual ~DescriptorManagement( VOID ) noexcept = default;

    public:
        /**
         * @brief 装载
         */
        virtual auto load( VOID ) const -> VOID = 0;
        /**
         * @brief 读取
         * @return 指向Descriptor Table的指针
         */
        virtual auto read( VOID ) const -> Lib::Types::Ptr< D > = 0;

    protected:
        /**
         * @brief 描述符表属性记录
         */
        struct DescriptorAttribute
        {
            Lib::Types::uint16_t size { };
            Lib::Types::Ptr< D > descriptor { };
        } _packed xdtr;
    };
}
