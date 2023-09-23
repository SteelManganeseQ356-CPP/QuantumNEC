#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Architecture::Device {
    DeviceManagement::DeviceManagement( VOID ) :
        TimerDeviceManagement { } {
        // Device Management 初始化
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the Architecture Driver Management." << Lib::IO::endl;
    }
    DeviceManagement::~DeviceManagement( VOID ) {
    }
}