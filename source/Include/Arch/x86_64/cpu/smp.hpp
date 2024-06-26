#pragma once
#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/Base/deflib.hpp>
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Architecture::CPU {
    PUBLIC class SMPManagement
    {
    public:
        explicit( TRUE ) SMPManagement( VOID ) noexcept;
        virtual ~SMPManagement( VOID ) noexcept = default;
    };
}
