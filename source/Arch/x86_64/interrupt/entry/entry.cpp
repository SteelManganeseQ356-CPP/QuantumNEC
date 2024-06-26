#include "Arch/x86_64/interrupt/entry/clock.hpp"
#include "Arch/x86_64/interrupt/entry/rtc.hpp"
#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    InterruptEntryRegister::InterruptEntryRegister( VOID ) noexcept :
        ClockEntry { }, SystemcallEntry { }, LocalApicEntry { }, RTCEntry { } {
    }
    InterruptEntryRegister::~InterruptEntryRegister( VOID ) noexcept {
    }
}