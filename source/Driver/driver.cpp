#include <Driver/driver.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Driver {
    DriverManagement::DriverManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) :
        GraphicsManagement { _config }, AcpiManagement { _config } {
    }
    DriverManagement::~DriverManagement( VOID ) {
        using namespace QuantumNEC::Lib::IO;
        sout[ ostream::HeadLevel::OK ] << "Initialize the Device Driver." << endl;
    }
}
