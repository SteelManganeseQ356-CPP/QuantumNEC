#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::PIT {
    PITManagement::PITManagement( VOID ) noexcept :
#ifndef APIC
        P8254Management { },
#else
        HpetManagement { } {
#endif
}
PITManagement::~PITManagement( VOID ) noexcept {
}
}