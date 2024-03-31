#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Arch/x86_64/platform/descriptor.hpp>
#include <Arch/x86_64/platform/global.hpp>
#include <Lib/Base/deflib.hpp>
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::CPU {
    PUBLIC class SMPManagement
    {
    public:
        explicit( TRUE ) SMPManagement( VOID ) noexcept;
        virtual ~SMPManagement( VOID ) = default;
    };
}
