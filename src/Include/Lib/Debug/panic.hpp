#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/IO/Stream/iostream>
#include <Utils/asm.hpp>
PUBLIC namespace QuantumNEC::Lib::Debug {
    PUBLIC auto panic( IN Lib::Types::Ptr< CONST Lib::Types::char_t > message )->VOID;
    PUBLIC template < typename... Msgs >
    auto panic( IN Msgs... messages )->VOID {
        using Lib::IO::sout;
        using Lib::IO::ostream;
        using Lib::IO::endl;
        sout[ ostream::HeadLevel::DEBUG ] << "!!! ================== \tPanic Spin\t ================== !!!" << endl;
        sout[ ostream::HeadLevel::ERROR ];
        ( ( sout << '\t' << messages ), ... );
        endl( sout );
        sout[ ostream::HeadLevel::DEBUG ] << "!!! ================== \tPanic Spin\t ================== !!!" << endl;
        while ( TRUE )
            Utils::hlt( );
    }
}
#define Panic( ... ) QuantumNEC::Lib::Debug::panic( __FILE__, __LINE__, __func__, #__VA_ARGS__ )