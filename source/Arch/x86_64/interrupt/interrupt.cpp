#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Utils/asm.hpp>

PUBLIC namespace QuantumNEC::Architecture::Interrupt {
    InterruptManagement::InterruptManagement( VOID ) :
#ifndef APIC
        Pic8259aManagement { },
#else
        ApicManagement { },
#endif
        InterruptEntryRegister { }

    {
        // Interrupt Management 初始化
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the Interrupt Management." << Lib::IO::endl;
    }
    InterruptManagement::~InterruptManagement( VOID ) {
    }
    auto InterruptManagement::enable_interrupt( VOID )->InterruptStatus {
        InterruptStatus status { InterruptStatus::INTERRUPT_DISABLE };
        if ( get_interrupt( ) == InterruptStatus::INTERRUPT_ENABLE ) {
            status = InterruptStatus::INTERRUPT_ENABLE;
            return status;
        }
        else {
            status = InterruptStatus::INTERRUPT_DISABLE;
            Utils::sti( );
            return status;
        }
    }

    auto InterruptManagement::disable_interrupt( VOID )->InterruptStatus {
        InterruptStatus status { InterruptStatus::INTERRUPT_DISABLE };
        if ( get_interrupt( ) == InterruptStatus::INTERRUPT_ENABLE ) {
            status = InterruptStatus::INTERRUPT_ENABLE;
            Utils::cli( );
            return status;
        }
        else {
            status = InterruptStatus::INTERRUPT_DISABLE;
            return status;
        }
    }

    auto InterruptManagement::set_interrupt( IN Lib::Types::L_Ref< CONST InterruptStatus > status )->InterruptStatus {
        return status == InterruptStatus::INTERRUPT_ENABLE ? enable_interrupt( ) : disable_interrupt( );
    }

    auto InterruptManagement::get_interrupt( VOID )->InterruptStatus {
        return ( Utils::get_eflags( ) & 0x00000200 ) ? InterruptStatus::INTERRUPT_ENABLE : InterruptStatus::INTERRUPT_DISABLE;
    }
}