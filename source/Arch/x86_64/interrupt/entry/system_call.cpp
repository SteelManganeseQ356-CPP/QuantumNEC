#include <Arch/x86_64/platform/platform.hpp>
#include <Kernel/task.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PRIVATE auto ASMCALL syscall( IN Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > frame )->Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > {
        using Kernel::TaskManagement;
        Kernel::TaskManagement::save_frame( frame );
        Architecture::Platform::SyscallManagement::get_syscall_table( )[ frame->regs.rbx ]( const_cast< Lib::Types::Ptr< Architecture::CPU::InterruptFrame > >( frame ) );
        Lib::IO::sout << TaskManagement::ready_task->cpu_frame->rip << " " << TaskManagement::ready_task->name << "\n";
        Lib::IO::sout << frame->rip << "\n";
        return TaskManagement::ready_task->cpu_frame;
    }

    SystemcallEntry::SystemcallEntry( VOID ) noexcept {
        Architecture::CPU::InterruptDescriptorManagement::set_interrupt_handler( Architecture::Platform::SYSTEM_CALL_INTERRUPTS_INDEX, syscall );     // 注册系统调用中断入口函数
    }
}