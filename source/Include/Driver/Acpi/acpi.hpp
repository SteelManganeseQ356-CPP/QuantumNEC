#ifndef _DRIVER_HPP_
#error Do not include the header file. Please include driver.hpp
#endif
#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Driver::Acpi {
    PUBLIC class AcpiManagement
    {
    private:
        enum class ICSAttribute : Lib::Types::uint8_t {
            LOCAL_APIC = 0,
            I_O_APIC = 1,
            INTERRUPT_SOURCE_OVERRIDE = 2,
        };
        typedef struct
        {
            Lib::Types::uint32_t signature;
            Lib::Types::uint32_t length;
            Lib::Types::uint8_t reserved;
            Lib::Types::uint8_t checksum;
            Lib::Types::uint8_t OEMID[ 6 ];
            Lib::Types::uint8_t OEMTableID[ 8 ];
            Lib::Types::uint32_t OEMReserved;
            Lib::Types::uint32_t creatorID;
            Lib::Types::uint32_t creatorReserved;
        } _packed SDTHeader;
        typedef struct : SDTHeader
        {
            auto operator[]( Lib::Types::size_t index ) CONST->Lib::Types::L_Ref< CONST SDTHeader > {
                Lib::Types::Ptr< CONST Lib::Types::uint64_t > entries { reinterpret_cast< Lib::Types::Ptr< CONST Lib::Types::uint64_t > >( this + 1 ) };
                return *reinterpret_cast< Lib::Types::Ptr< CONST SDTHeader > >( entries + index );
            }
            auto size( VOID ) CONST->Lib::Types::size_t {
                return ( this->length - sizeof( Xsdt ) ) / sizeof( Lib::Types::uint64_t );
            }
        } _packed Xsdt;
        typedef struct : SDTHeader
        {
            Lib::Types::uint32_t localApicAddress;
            Lib::Types::uint32_t flags;
        } _packed Madt;
        typedef struct
        {
            ICSAttribute type;
            Lib::Types::uint8_t length;
        } _packed MadtICS;

    public:
        typedef struct : MadtICS
        {
            Lib::Types::uint8_t acpiProcessorUID;
            Lib::Types::uint8_t apicID;
            Lib::Types::uint32_t flags;
        } _packed LocalApic;
        typedef struct : MadtICS
        {
            Lib::Types::uint8_t IOApicID;
            Lib::Types::uint8_t reserved;
            Lib::Types::uint32_t IOApicAddress;
            Lib::Types::uint32_t globalSystemInterruptBase;
        } _packed IOApic;
        typedef struct : MadtICS
        {
            Lib::Types::uint8_t bus;
            Lib::Types::uint8_t source;
            Lib::Types::uint32_t globalSystemInterrupt;
            Lib::Types::uint16_t flags;
        } _packed InterruptSourceOverride;

    public:
        explicit( true ) AcpiManagement( IN CONST Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true );

    public:
        virtual ~AcpiManagement( VOID ) noexcept( true ) = default;

    public:
        template < typename T >
        auto isValid( VOID ) CONST->Lib::Types::BOOL;

    private:
        Lib::Types::Ptr< Lib::Types::Rsdp > rsdp;
        Lib::Types::Ptr< Xsdt > xsdt;
    };
    template <>
    auto AcpiManagement::isValid< Lib::Types::Rsdp >( VOID ) CONST->Lib::Types::BOOL;
}
