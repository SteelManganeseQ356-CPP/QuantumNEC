#include "Lib/Base/deflib.hpp"
#include <Arch/x86_64/platform/platform.hpp>
#include <Kernel/task.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Architecture {
    _C_LINK auto save_current_frame( IN CONST Kernel::Task::ProcessFrame * frame )->VOID;
    _C_LINK auto syscall_entry( IN Lib::Types::Ptr< CONST CPUs::InterruptFrame > frame )->IN Lib::Types::Ptr< CONST CPUs::InterruptFrame > {
        using Kernel::Task;
        save_current_frame( frame );
        return Syscall::get_syscall_table( )[ frame->regs.rax ]( const_cast< Lib::Types::Ptr< CPUs::InterruptFrame > >( frame ) );
    }

    SystemcallEntry::SystemcallEntry( VOID ) noexcept {
        Architecture::CPUs::set_interrupt_handler( IRQ_SYSTEM_CALL, syscall_entry );     // 注册系统调用中断入口函数
    }
}
