#pragma once
#include <Lib/Base/deflib.hpp>
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
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
    class Descriptor
    {
    public:
        explicit( TRUE ) Descriptor( IN Lib::Types::Ptr< D > _descriptor, IN Lib::Types::uint16_t _descriptorCount ) noexcept :
            xdtr { Lib::Types::uint16_t( sizeof( D ) * _descriptorCount - 1 ), _descriptor } {
        }
        virtual ~Descriptor( VOID ) noexcept = default;

    public:
        /**
         * @brief 装载描述符
         */
        virtual auto load( VOID ) const -> VOID = 0;
        /**
         * @brief 读取
         * @return 指向Descriptor Table的指针
         */
        virtual auto read( VOID ) const -> Lib::Types::Ptr< D > = 0;

    protected:
        /**
         * @brief 描述符表引索
         */
        struct DescriptorRegister
        {
            Lib::Types::uint16_t size { };
            Lib::Types::Ptr< D > descriptor { };
        } _packed xdtr;
    };
}
