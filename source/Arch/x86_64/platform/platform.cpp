#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Kernel/task.hpp>
_C_LINK {
    EXTERN QuantumNEC::Lib::Types::byte_t stack_space[];
}
PUBLIC namespace QuantumNEC::Architecture {
    x86_64Architecture::x86_64Architecture( IN Lib::Types::Ptr< Lib::Types::BootConfig > ) :
        CPUManagement { },
        DeviceManagement { },
        InterruptManagement { } {
        this->kernel_stack_space = stack_space;
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::INFO ] << "QuantumNEC-System-Architecture-Mode : intel IA-32e(x86-64)." << Lib::IO::endl;
    }
    Platform::ControlRegisterFrame::ControlRegisterFrame( VOID ) noexcept {
        this->cr0 = x86_64Architecture::read_cr0( );
        this->cr2 = x86_64Architecture::read_cr2( );
        this->cr3 = x86_64Architecture::read_cr3( );
        this->cr4 = x86_64Architecture::read_cr4( );
    }
    Platform::ControlRegisterFrame::~ControlRegisterFrame( VOID ) noexcept {
    }
}