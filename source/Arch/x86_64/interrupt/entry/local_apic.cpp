#include "Arch/x86_64/platform/global.hpp"
#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    PRIVATE auto ASMCALL local_apic_spurious( IN Lib::Types::Ptr< CONST CPUs::InterruptFrame > frame )->Lib::Types::Ptr< CONST CPUs::InterruptFrame > {
        return frame;
    }

    PRIVATE auto ASMCALL local_apic_error( IN Lib::Types::Ptr< CONST CPUs::InterruptFrame > frame )->Lib::Types::Ptr< CONST CPUs::InterruptFrame > {
        return frame;
    }
    LocalApicEntry::LocalApicEntry( VOID ) noexcept {
        Architecture::CPUs::set_interrupt_handler( IRQ_LOCAL_APIC_SPURIOUS, local_apic_spurious );     // 注册Local APIC 中断入口函数
        Architecture::CPUs::set_interrupt_handler( IRQ_LOCAL_APIC_ERROR, local_apic_error );           // 注册Local APIC 中断入口函数
    }
}