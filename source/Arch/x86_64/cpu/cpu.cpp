#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/Base/deflib.hpp>
#include <Utils/asm.hpp>
#include <Lib/IO/Stream/iostream>

PUBLIC namespace QuantumNEC::Architecture::CPU {
    
    CPUManagement::CPUManagement( VOID ) :
        InterruptDescriptorManagement { idt, Architecture::Platform::GDT_MAX },
        GlobalDescriptorManagement { gdt, Architecture::Platform::IDT_MAX } {
        // CPU Management初始化
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the CPU Management." << Lib::IO::endl;
    }
    auto CPUManagement::cpuInfo( VOID )->VOID {
    }
    auto CPUManagement::invlpg( IN Lib::Types::Ptr< VOID > address )->VOID {
        ASM( "invlpg (%0)" : : "r"( address ) : "memory" );
    }
}