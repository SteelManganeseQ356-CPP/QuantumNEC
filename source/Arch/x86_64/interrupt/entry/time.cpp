#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Kernel/task.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PRIVATE auto ASMCALL time( IN Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > frame )->Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > {
        using namespace Kernel;
        using namespace Kernel::Task;
        Architecture::Interrupt::InterruptManagement::eoi( frame->irq );

        volatile Lib::Types::Ptr< TaskManagement::ProcessPCB > current { TaskManagement::get_current< TaskManagement::ProcessPCB >( ) };
        Kernel::TaskManagement::save_frame( frame );

        if ( !current->ticks ) {
            TaskManagement::schedule( );
            current->ticks = current->priority;
            return TaskManagement::ready_task->cpu_frame;
        }
        else {
            current->ticks--;
            return frame;
        }
        return frame;
    }
    TimeEntry::TimeEntry( VOID ) noexcept {
        Architecture::CPU::InterruptDescriptorManagement::set_interrupt_handler( 0x20, time );     // 注册时间中断入口函数
    }
}