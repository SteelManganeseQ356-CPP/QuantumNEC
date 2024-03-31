#include <Arch/x86_64/platform/platform.hpp>
#include <Utils/asm.hpp>
#include <Lib/IO/Stream/iostream>
#include <Kernel/task.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    Lib::Types::uint64_t global_ticks { 0 };
    PUBLIC auto time_interrupt_entry( IN Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > frame )->VOID {
        using namespace Kernel;
        using namespace Kernel::Task;
        Architecture::Interrupt::Pic8259aManagement::eoi( frame->irq );
        global_ticks++;
        Lib::Types::Ptr< TaskManagement::ProcessPCB > current { TaskManagement::get_current< TaskManagement::ProcessPCB >( ) };
        if ( current->stack_magic != TASK_STACK_MAGIC ) {
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::ERROR ] << "Task stack error!" << Lib::IO::endl;
            while ( TRUE )
                ;
        }
        current->counter++;
        if ( !current->ticks ) {
            TaskManagement::schedule( );
        }
        else {
            current->ticks--;
        }
        return;
    }
    TimeEntry::TimeEntry( VOID ) {
        Architecture::CPU::InterruptDescriptorManagement::set_interrupt_handler( 0x20, time_interrupt_entry );     // 注册时间中断入口函数
    }
}