#include <Lib/STL/string>
PUBLIC namespace QuantumNEC::Lib::STL {
    basic_string< Lib::Types::char_t >::basic_string( VOID ) {
    }
    basic_string< Lib::Types::char_t >::basic_string( IN Lib::Types::Ptr< CONST Lib::Types::char_t >  ) {
        // 深拷贝
    }
    basic_string< Lib::Types::char_t >::basic_string( IN Lib::Types::L_Ref< basic_string< Lib::Types::char_t > >  ) {
        //
    }
    basic_string< Lib::Types::char_t >::~basic_string( VOID ) {
        // 析构str
    }
    auto basic_string< Lib::Types::char_t >::c_str( VOID )->Lib::Types::Ptr< Lib::Types::char_t > {
        return this->__str;
    }
    auto basic_string< Lib::Types::char_t >::operator[]( Lib::Types::size_t size ) CONST->Lib::Types::L_Ref< CONST Lib::Types::char_t > {

        if ( size > this->__str__length )
            return this->__str[this->__str__length];
        return this->__str[ size ];
    }

}
