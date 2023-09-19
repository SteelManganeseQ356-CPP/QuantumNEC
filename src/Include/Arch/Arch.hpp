#pragma once
#include <Arch/ArchInfo.hpp>
#if SYSTEM_ARCH == x86_64
#include <Arch/x86_64/platform/platform.hpp>
#elif SYSTEM_ARCH == risc_v
#include <Arch/risc-v/platform/platform.hpp>
#elif SYSTEM_ARCH == arm
#include <Arch/aarch64/platform/platform.hpp>
#endif
#include <Lib/Types/Uefi.hpp>
#include <Lib/STL/concepts>

PUBLIC namespace QuantumNEC::Architecture {
    template < typename Arch >
        requires Lib::STL::derived_from< Arch, typename Architecture::CPU::CPUManagement >
                 || Lib::STL::derived_from< Arch, typename Architecture::Driver::DriverManagement >
                 || Lib::STL::derived_from< Arch, typename Architecture::Interrupt::InterruptManagement >
    class ArchitectureManagement :
        Arch
    {
    public:
        explicit( true ) ArchitectureManagement( Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true ) :
            Arch { _config } {
        }

    public:
        virtual ~ArchitectureManagement( ) noexcept( true ) = default;

    private:
    };
}
