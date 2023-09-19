#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Architecture {
    x86_64Architecture::x86_64Architecture( IN Lib::Types::Ptr< Lib::Types::BootConfig > ) :
        CPUManagement { },
        DriverManagement { },
        InterruptManagement { } {
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::INFO ] << "QuantumNEC-System-Architecture-Mode : intel IA-32e(x86-64)." << Lib::IO::endl;
    }
}