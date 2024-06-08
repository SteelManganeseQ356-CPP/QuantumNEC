#pragma once
#ifndef _DRIVER_HPP_
#define _DRIVER_HPP_
#endif
#include <Driver/Display/graphics.hpp>
#include <Driver/Acpi/acpi.hpp>
#include <Driver/Audio/audio.hpp>
PUBLIC namespace QuantumNEC::Driver {
    class DriverManagement :
        public GraphicsManagement,
        public AcpiManagement
    {
    public:
        explicit( TRUE ) DriverManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( TRUE );
        virtual ~DriverManagement( VOID ) noexcept( TRUE );

    private:
    };
}