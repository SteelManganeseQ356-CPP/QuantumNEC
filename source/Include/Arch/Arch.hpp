#pragma once
#include <Arch/ArchInfo.hpp>
#if SYSTEM_ARCH == x86_64
#include <Arch/x86_64/platform/platform.hpp>
#elif SYSTEM_ARCH == risc_v
#include <Arch/risc-v/platform/platform.hpp>
#elif SYSTEM_ARCH == arm64
#include <Arch/aarch64/platform/platform.hpp>
#endif
#include <Lib/Types/Uefi.hpp>
#include <concepts>

PUBLIC namespace QuantumNEC::Architecture {
    template < typename Arch >
        requires requires { typename Arch; }
                 && std::derived_from< Arch, typename Architecture::CPU::CPUManagement >
                 && std::derived_from< Arch, typename Architecture::Device::DeviceManagement >
                 && std::derived_from< Arch, typename Architecture::Interrupt::InterruptManagement >
    class ArchitectureManagement : public Arch
    {
    public:
        explicit( TRUE ) ArchitectureManagement( Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( TRUE ) :
            Arch { _config } {
        }

    public:
        virtual ~ArchitectureManagement( VOID ) noexcept( TRUE ) {
        }

    private:
    };
}
