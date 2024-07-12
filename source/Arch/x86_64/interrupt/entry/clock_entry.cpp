#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Kernel/task.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    _C_LINK auto save_current_frame( IN CONST Kernel::Task::ProcessFrame * frame )->VOID;
    PRIVATE auto ASMCALL clock_handler( IN Lib::Types::Ptr< CONST Architecture::CPUs::InterruptFrame > frame )->Lib::Types::Ptr< CONST Architecture::CPUs::InterruptFrame > {
        using namespace Kernel;
        Architecture::Interrupt::eoi( frame->vector );

        volatile Lib::Types::Ptr< Task::ProcessPCB > current { Task::get_current< Task::ProcessPCB >( ) };
        save_current_frame( frame );

        if ( !current->ticks ) {
            Task::schedule( );
            current->ticks = current->priority;
            return Task::ready_task->cpu_frame;
        }
        else {
            current->ticks--;
            return frame;
        }
        return frame;
    }
    ClockEntry::ClockEntry( VOID ) noexcept {
        Architecture::CPUs::set_interrupt_handler( IRQ_CLOCK, clock_handler );     // 注册时间中断入口函数
    }
}