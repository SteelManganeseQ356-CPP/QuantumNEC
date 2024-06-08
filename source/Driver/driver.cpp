#include <Driver/driver.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Driver {
    DriverManagement::DriverManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept :
        GraphicsManagement { _config }, AcpiManagement { _config } {
    }
    DriverManagement::~DriverManagement( VOID ) noexcept {
        using namespace QuantumNEC::Lib::IO;
        sout[ ostream::HeadLevel::OK ] << "Initialize the Driver." << endl;
    }
}
