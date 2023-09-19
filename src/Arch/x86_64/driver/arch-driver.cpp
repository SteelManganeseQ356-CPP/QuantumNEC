#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Architecture::Driver {
    DriverManagement::DriverManagement( VOID ) :
        TimerDriverManagement { } {
        // Driver Management 初始化
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the Architecture Driver Management." << Lib::IO::endl;
    }
    DriverManagement::~DriverManagement( VOID ) {
    }
}