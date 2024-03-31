#pragma once
#ifndef _DRIVER_HPP_
#error Do not include the header file. Please include driver.hpp
#endif
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Driver {
    PUBLIC class AudioManagement {
        explicit( TRUE ) AudioManagement( ) noexcept( TRUE );
    };
}