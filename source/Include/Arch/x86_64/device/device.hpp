#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Arch/x86_64/device/fpu.hpp>
#include <Arch/x86_64/device/clock.hpp>
#include <Arch/x86_64/device/graphics.hpp>
#include <Arch/x86_64/device/acpi.hpp>
#include <Arch/x86_64/device/audio.hpp>
#include <Arch/x86_64/device/beep.hpp>
#include <Arch/x86_64/device/cmos.hpp>

PUBLIC namespace QuantumNEC::Architecture::Device {
    PUBLIC class DeviceManagement :
        public GraphicsManagement,
        public AcpiManagement,
        public Beep,
        public CMOS,
        public FPUDriverManagement
    {
    public:
        explicit DeviceManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept;
        virtual ~DeviceManagement( VOID ) noexcept;

    private:
    };
}