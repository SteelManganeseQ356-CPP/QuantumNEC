#include <Kernel/kernel.hpp>
#include <Kernel/memory.hpp>
#include <Arch/Arch.hpp>
#include <Driver/driver.hpp>
#include <Lib/IO/Stream/iostream>
#include <Utils/asm.hpp>
using namespace QuantumNEC;
using namespace QuantumNEC::Lib;
using namespace QuantumNEC::Lib::Types;
_C_LINK auto microKernelEntry( IN Ptr< BootConfig > config ) -> SystemStatus {
    SystemStatus Status { SYSTEM_SUCCESS };
    Driver::DriverManagement { config };     // 驱动初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::INFO ] << "This is QuantumNEC-OS. Build number 10006." << Lib::IO::endl;
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the Memory Management." << Lib::IO::endl;
    Kernel::MemoryManagement { config };     // 内存管理初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the Memory Management." << Lib::IO::endl;
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the System Architecture." << Lib::IO::endl;
    Architecture::ArchitectureManagement< TARGET_ARCH > { config };     // 系统架构初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the System Architecture." << Lib::IO::endl;
    
    while ( TRUE )
        ;
    return Status;
}
