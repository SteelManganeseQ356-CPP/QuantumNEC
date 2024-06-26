
#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    PRIVATE auto ASMCALL real_time_clock( IN Lib::Types::Ptr< CONST CPUs::InterruptFrame > frame )->Lib::Types::Ptr< CONST CPUs::InterruptFrame > {
        Architecture::Interrupt::eoi( frame->irq );
        return frame;
    }

    RTCEntry::RTCEntry( VOID ) noexcept {
        Architecture::CPUs::set_interrupt_handler( RTC_INTERRUPTS_INDEX, real_time_clock );     // 注册RTC中断入口函数
    }
}