#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#ifndef _ARCH_DRIVER_HPP_
#define _ARCH_DRIVER_HPP_
#endif
#include <Lib/Types/Uefi.hpp>
#include <Arch/x86_64/device/timer.hpp>
PUBLIC namespace QuantumNEC::Architecture::Device {
    PUBLIC class DeviceManagement : TimerDeviceManagement
    {
    public:
        explicit( true ) DeviceManagement( VOID ) noexcept( true );

    public:
        virtual ~DeviceManagement( VOID ) noexcept( true );

    private:
    };
}