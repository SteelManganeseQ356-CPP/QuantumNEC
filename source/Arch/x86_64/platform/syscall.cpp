#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Kernel/task.hpp>
#include <Lib/STL/string>
PUBLIC namespace QuantumNEC::Architecture::Platform {
    SyscallManagement::SyscallManagement( VOID ) noexcept {
        this->set_syscall_table( 0, syscall_send_receive );
        this->set_syscall_table( 1, syscall_send_receive );
        this->set_syscall_table( 2, syscall_send_receive );
    }
    auto SyscallManagement::set_syscall_table( IN Lib::Types::uint64_t index, SyscallEntry entry )->VOID {
        syscall_table[ index ] = entry;
    }
    auto SyscallManagement::syscall_send_receive( IN Lib::Types::Ptr< Architecture::CPU::InterruptFrame > frame )->VOID {
        using namespace Kernel;
        using namespace Kernel::Task;
        Lib::Types::uint64_t function { frame->regs.rbx };
        SyscallStatus status { SyscallStatus::ERROR };
        Lib::Types::uint64_t source_destination { frame->regs.rcx };
        switch ( Lib::Types::Ptr< MessageManagement > message { reinterpret_cast< decltype( message ) >( frame->regs.rdx ) }; function ) {
        case static_cast< Lib::Types::uint64_t >( SyscallFunction::MESSAGE_SEND ):
            status = message->send( source_destination );
            break;
        case static_cast< Lib::Types::uint64_t >( SyscallFunction::MESSAGE_RECEIVE ):
            status = message->receive( source_destination );
            break;
        default:
            status = SyscallStatus::NO_SYSCALL;
            break;
        }
        if ( status != SyscallStatus::SUCCESS ) {
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::ERROR ] << TaskManagement::get_current< TaskManagement::ProcessPCB >( )->name << " called " << static_cast< Lib::Types::uint32_t >( function ) << " syscall error!"
                                                                << " to PID:" << source_destination << "(" << static_cast< Lib::Types::int32_t >( status ) << ")" << Lib::IO::endl;
        }
        frame->regs.rax = static_cast< Lib::Types::uint64_t >( status );
    }
}