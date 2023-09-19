#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Driver::Acpi {
    PUBLIC constexpr auto SIGN_16( auto A, auto B )->decltype( auto ) {
        return ( A ) | ( B << 8 );
    }
    PUBLIC constexpr auto SIGN_32( auto A, auto B, auto C, auto D )->decltype( auto ) {
        return SIGN_16( A, B ) | ( SIGN_16( C, D ) << 16 );
    }
    PUBLIC constexpr auto SIGN_64( auto A, auto B, auto C, auto D, auto E, auto F, auto G, auto H )->decltype( auto ) {
        return SIGN_32( A, B, C, D ) | ( (Lib::Types::uint64_t)SIGN_32( E, F, G, H ) << 32 );
    }
    PUBLIC constexpr CONST auto RSDP_SIG { SIGN_64( 'R', 'S', 'D', ' ', 'P', 'T', 'R', ' ' ) };
    PUBLIC constexpr CONST auto XSDT_SIG { SIGN_32( 'X', 'S', 'D', 'T' ) };
    PUBLIC constexpr CONST auto MADT_SIG { SIGN_32( 'A', 'P', 'I', 'C' ) };
    PUBLIC class AcpiManagement
    {
    public:
        explicit( true ) AcpiManagement( IN CONST Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true );

    public:
        virtual ~AcpiManagement( VOID ) noexcept( true ) = default;

    private:
        auto checkSum( IN Lib::Types::size_t size ) -> Lib::Types::BOOL;
        auto getEntry( IN Lib::Types::uint32_t sign ) -> Lib::Types::Ptr< VOID >;

    private:
        Lib::Types::Ptr< Lib::Types::RsdpConfig > rsdp;
        Lib::Types::Ptr< Lib::Types::XsdtConfig > xsdt;
    };
}