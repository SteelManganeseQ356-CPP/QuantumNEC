#include <Arch/x86_64/platform/platform.hpp>
#include <Kernel/task.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Architecture {
    PRIVATE auto ASMCALL syscall( IN Lib::Types::Ptr< CONST CPUs::InterruptFrame > frame )->Lib::Types::Ptr< CONST CPUs::InterruptFrame > {
        using Kernel::Task;
        Task::save_frame( frame );
        Syscall::get_syscall_table( )[ frame->regs.rbx ]( const_cast< Lib::Types::Ptr< CPUs::InterruptFrame > >( frame ) );

        Lib::IO::sout << Task::ready_task->cpu_frame->rip << " " << Task::ready_task->name << "\n";
        Lib::IO::sout << frame->rip << "\n";

        return Task::ready_task->cpu_frame;
    }

    SystemcallEntry::SystemcallEntry( VOID ) noexcept {
        Architecture::CPUs::set_interrupt_handler( SYSTEM_CALL_INTERRUPTS_INDEX, syscall );     // 注册系统调用中断入口函数
    }
}