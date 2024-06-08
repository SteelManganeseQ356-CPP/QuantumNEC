#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    PRIVATE auto ASMCALL local_apic_spurious( IN Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > frame )->Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > {
        return frame;
    }

    PRIVATE auto ASMCALL local_apic_error( IN Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > frame )->Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > {
        return frame;
    }
    LocalApicEntry::LocalApicEntry( VOID ) noexcept {
        Architecture::CPU::InterruptDescriptorManagement::set_interrupt_handler( Architecture::Platform::LOCAL_APIC_SPURIOUS_INTERRUPTS_INDEX, local_apic_spurious );     // 注册Local APIC 中断入口函数
        Architecture::CPU::InterruptDescriptorManagement::set_interrupt_handler( Architecture::Platform::LOCAL_APIC_ERROR_INTERRUPTS_INDEX, local_apic_error );           // 注册Local APIC 中断入口函数
    }
}