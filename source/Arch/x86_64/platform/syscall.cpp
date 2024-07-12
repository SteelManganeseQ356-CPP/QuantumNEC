
#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Kernel/task.hpp>
#include <Lib/STL/string>
#include <Lib/Debug/panic.hpp>
PUBLIC QuantumNEC::Architecture::Syscall::SyscallEntry syscall_table[ QuantumNEC::Architecture::NR_SYS_CALL ] { };
PUBLIC namespace QuantumNEC::Architecture {
    using namespace Kernel;
    /**
     * 第一个系统调用
     * 它的作用是进行进程间通信，消息发送/接收
     */
    PRIVATE auto syscall_send_receive( IN Lib::Types::Ptr< CPUs::InterruptFrame > frame )->Lib::Types::Ptr< CPUs::InterruptFrame > {
        Lib::Types::uint64_t function { frame->regs.rax };
        Syscall::SyscallStatus status { Syscall::SyscallStatus::ERROR };
        Lib::Types::uint64_t source_destination { frame->regs.rdi };
        switch ( Lib::Types::Ptr< Message > message { reinterpret_cast< decltype( message ) >( frame->regs.rsi ) }; function ) {
        case static_cast< Lib::Types::uint64_t >( Syscall::SyscallFunction::MESSAGE_SEND ):
            status = message->send( source_destination );
            break;
        case static_cast< Lib::Types::uint64_t >( Syscall::SyscallFunction::MESSAGE_RECEIVE ):
            status = message->receive( source_destination );
            break;
        default:
            status = Syscall::SyscallStatus::NO_SYSCALL;
            break;
        }
        if ( status != Syscall::SyscallStatus::SUCCESS ) {
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::ERROR ] << Task::get_current< Task::ProcessPCB >( )->name << " called " << static_cast< Lib::Types::uint32_t >( function ) << " syscall error!"
                                                                << " to PID:" << source_destination << "(" << static_cast< Lib::Types::int32_t >( status ) << ")" << Lib::IO::endl;
            while ( TRUE )
                ;
        }
        Task::ready_task->cpu_frame->regs.rax = static_cast< Lib::Types::uint64_t >( status );
        return Task::ready_task->cpu_frame;
    }

    Syscall::Syscall( VOID ) noexcept {
        this->set_syscall_table( 0, syscall_send_receive );
        this->set_syscall_table( 1, syscall_send_receive );
        this->set_syscall_table( 2, syscall_send_receive );
    }
    auto Syscall::get_syscall_table( VOID )->Lib::Types::Ptr< SyscallEntry > {
        return syscall_table;
    }
    auto Syscall::set_syscall_table( IN Lib::Types::uint64_t index, SyscallEntry entry )->VOID {
        syscall_table[ index ] = entry;
    }
}