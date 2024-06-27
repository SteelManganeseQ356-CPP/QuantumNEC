#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    Hpet ::Hpet( VOID ) noexcept {
        /* 设置 Hpet 里时间周期的 */
        HpetInfo *info { reinterpret_cast< decltype( info ) >( Device::hpet->hpet_address.address ) };
        info->generalConfiguration |= 1;
    }
    Hpet::~Hpet( VOID ) noexcept {
    }
}