#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    InterruptEntryRegister::InterruptEntryRegister( VOID ) noexcept :
        TimeEntry { }, SystemcallEntry { }, LocalApicEntry { } {
    }
    InterruptEntryRegister::~InterruptEntryRegister( VOID ) noexcept {
    }
}