
#include "Arch/x86_64/interrupt/interrupt.hpp"
#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PRIVATE auto ASMCALL real_time_clock( IN Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > frame )->Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > {
        Architecture::Interrupt::InterruptManagement::eoi( frame->irq );
        return frame;
    }

    RTCEntry::RTCEntry( VOID ) noexcept {
        Architecture::CPU::InterruptDescriptorManagement::set_interrupt_handler( Architecture::Platform::RTC_INTERRUPTS_INDEX, real_time_clock );     // 注册RTC中断入口函数
    }
}