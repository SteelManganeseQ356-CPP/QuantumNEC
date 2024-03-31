#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Lib/Types/Uefi.hpp>
PUBLIC auto operator new( IN unsigned long int ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID >;
PUBLIC auto operator new[]( IN unsigned long int ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID >;
PUBLIC auto operator new( IN unsigned long int, IN QuantumNEC::Lib::Types::Ptr< VOID > ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID >;
PUBLIC auto operator delete( IN QuantumNEC::Lib::Types::Ptr< VOID >, IN unsigned long int ) noexcept -> VOID;
PUBLIC auto operator delete( IN QuantumNEC::Lib::Types::Ptr< VOID > ) noexcept -> VOID;
PUBLIC auto operator delete[]( IN QuantumNEC::Lib::Types::Ptr< VOID > ) noexcept -> VOID;