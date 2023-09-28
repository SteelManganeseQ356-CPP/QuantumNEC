#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Utils/asm.hpp>

PUBLIC namespace QuantumNEC::Architecture::Interrupt {
    InterruptManagement::InterruptManagement( VOID ) :
#ifndef APIC
        Pic8259aManagement {
    }
#else
        ApicManagement { }
#endif
    {
        /* 开启中断 */
        //Utils::sti( );
        // Interrupt Management 初始化
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the Interrupt Management." << Lib::IO::endl;
    }
    InterruptManagement::~InterruptManagement( VOID ) {
    }
    auto time( ::QuantumNEC::Lib::Types::Ptr< CONST ::QuantumNEC::Architecture::CPU::InterruptFrame > frame )->Lib::Types::uint8_t {
        InterruptManagement::eoi( frame->irq );
        return 0;
    }
}