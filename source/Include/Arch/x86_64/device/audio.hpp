#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC namespace QuantumNEC::Architecture::Device {
    PUBLIC class AudioManagement
    {
    public:
        explicit AudioManagement( VOID ) noexcept;
        virtual ~AudioManagement( VOID ) noexcept;
    };
}