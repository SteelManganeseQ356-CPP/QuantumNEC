#include <Kernel/kernel.hpp>
#include <Kernel/memory.hpp>
#include <Arch/Arch.hpp>
#include <Driver/Display/graphics.hpp>
#include <Lib/IO/Stream/iostream>
#include <Utils/asm.hpp>
using namespace QuantumNEC;
using namespace QuantumNEC::Lib;
using namespace QuantumNEC::Lib::Types;
_C_LINK SystemStatus
microKernelEntry( IN Ptr< BootConfig > config ) {
    SystemStatus Status { SYSTEM_SUCCESS };
    Driver::Display::GraphicsManagement { config }.windowInitialize( );     // 窗口初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::INFO ] << "This is QuantumNEC-OS. Build number 10006." << Lib::IO::endl;

    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the QuantumNEC-System-Kernel." << Lib::IO::endl;
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the QuantumNEC-System-Architecture." << Lib::IO::endl;
    Architecture::ArchitectureManagement< TARGET_ARCH > { config };     // 系统内核架构初始化

    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the QuantumNEC-System-Architecture." << Lib::IO::endl;
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the QuantumNEC-System-Memory Management." << Lib::IO::endl;
    Kernel::MemoryManagement { config };     // 内存管理初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the QuantumNEC-System-Memory Management." << Lib::IO::endl;
    
    while ( TRUE )
        ;
    return Status;
}
