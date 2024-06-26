#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture::Device {
    Beep::Beep( VOID ) noexcept {
    }
    Beep::~Beep( VOID ) noexcept {
    }
    auto Beep::enable_beep( VOID )->VOID {
        using namespace Architecture;
        if ( !beeping ) {
            Architecture::CPU::CPUManagement::io_out8( SPEAKER_REG, Architecture::CPU::CPUManagement::io_in8( SPEAKER_REG ) | 0x03 );
        }
        beeping = jiffies + 5;
    }

    auto Beep::disable_beep( VOID )->VOID {
        using namespace Architecture;
        if ( beeping && jiffies > beeping ) {
            Architecture::CPU::CPUManagement::io_out8( SPEAKER_REG, Architecture::CPU::CPUManagement::io_in8( SPEAKER_REG ) & 0xfc );
            beeping = 0;
        }
    }
}