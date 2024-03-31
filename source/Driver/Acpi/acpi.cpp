#include <Driver/driver.hpp>
#include <Lib/Debug/panic.hpp>
#include <Lib/STL/string>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace {
    PUBLIC using namespace QuantumNEC;
    PUBLIC using namespace QuantumNEC::Lib::Types;
    PUBLIC using namespace QuantumNEC::Lib::IO;
    PRIVATE consteval auto SIGN_16( auto A, auto B )->decltype( auto ) {
        return ( A ) | ( B << 8 );
    }
    PRIVATE consteval auto SIGN_32( auto A, auto B, auto C, auto D )->decltype( auto ) {
        return SIGN_16( A, B ) | ( SIGN_16( C, D ) << 16 );
    }
    PRIVATE consteval auto SIGN_64( auto A, auto B, auto C, auto D, auto E, auto F, auto G, auto H )->decltype( auto ) {
        return SIGN_32( A, B, C, D ) | ( (Lib::Types::uint64_t)SIGN_32( E, F, G, H ) << 32 );
    }
    PRIVATE constexpr auto bytes_sum_total( IN Lib::Types::Ptr< CONST uint8_t > data, IN size_t bytes )->decltype( auto ) {
        uint8_t sum { };
        for ( size_t i { }; i < bytes; ++i ) {
            sum += data[ i ];
        }
        return sum;
    }
    PRIVATE constexpr CONST auto GLOBAL_SYSTEM_INTERRUPT_COUNT { 24 };
    PRIVATE constexpr CONST auto RSDP_SIGNED { SIGN_64( 'R', 'S', 'D', ' ', 'P', 'T', 'R', ' ' ) };
    PRIVATE constexpr CONST auto XSDT_SIGNED { SIGN_32( 'X', 'S', 'D', 'T' ) };
    PRIVATE constexpr CONST auto MADT_SIGNED { SIGN_32( 'A', 'P', 'I', 'C' ) };
    PRIVATE constexpr CONST auto APIC_SIGNED { SIGN_32( 'A', 'P', 'I', 'C' ) };
    PRIVATE constexpr CONST auto FADT_SIGNED { SIGN_32( 'F', 'A', 'C', 'P' ) };
    PRIVATE uint8_t global_system_interrupt[ GLOBAL_SYSTEM_INTERRUPT_COUNT ];
}
PUBLIC namespace QuantumNEC::Driver::Acpi {
    AcpiManagement::AcpiManagement( IN CONST Lib::Types::Ptr< Lib::Types::BootConfig > _config ) :
        rsdp { _config->AcpiData.rsdpTable },
        xsdt { reinterpret_cast< Lib::Types::Ptr< Xsdt > >( this->rsdp->xsdtAddress ) } {
        using namespace Lib::Types;
        auto checkSum { []( IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::size_t size ) -> Lib::Types::BOOL {
            Lib::Types::uint8_t sum { };
            for ( size_t i = 0; i < size; i++ ) {
                sum += ( reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( buffer ) )[ i ];
            }
            return !sum;
        } };
        auto getEntry = [ &, this ]( Lib::Types::uint32_t signature ) -> Lib::Types::Ptr< VOID > {
            Lib::Types::size_t size { this->xsdt->size( ) };
            Lib::Types::Ptr< SDTHeader > header { };
            Lib::Types::Ptr< Lib::Types::uint64_t > address { reinterpret_cast< decltype( address ) >( (Lib::Types::uint64_t)this->xsdt + sizeof( Xsdt ) ) };
            for ( size_t i { }; i < size; i++ ) {
                header = reinterpret_cast< decltype( header ) >( address[ i ] );
                if ( header->signature == signature ) {
                    if ( checkSum( header, header->length ) )
                        return reinterpret_cast< Lib::Types::Ptr< VOID > >( header );
                }
            }
            return NULL;
        };
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the advanced configuration power interface." << Lib::IO::endl;
        if ( !this->is_valid< Rsdp >( ) ) {
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::ERROR ] << "RSD pointer found is not valid." << Lib::IO::endl;
            while ( true )
                ;
        }
        if ( this->xsdt->signature != ::XSDT_SIGNED || !checkSum( this->xsdt, this->xsdt->length ) ) {
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::ERROR ] << "XSDT is not valid." << Lib::IO::endl;
            while ( true )
                ;
        }
        Lib::Types::Ptr< Madt > madt { reinterpret_cast< Lib::Types::Ptr< Madt > >( getEntry( ::APIC_SIGNED ) ) };
        if ( !madt ) {
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::ERROR ] << "Can not find madt." << Lib::IO::endl;
            while ( true )
                ;
        }
        Lib::Types::int64_t length { static_cast< decltype( length ) >( madt->length - sizeof *madt ) };
        Lib::Types::Ptr< MadtICS > ICS { reinterpret_cast< decltype( ICS ) >( ( Lib::Types::uint64_t )( madt ) + sizeof *madt ) };
        Lib::STL::memset( ::global_system_interrupt, 0xFF, ::GLOBAL_SYSTEM_INTERRUPT_COUNT );
        while ( length > 0 ) {
            if ( ICS->type == ICSAttribute::LOCAL_APIC ) {
                Lib::Types::Ptr< LocalApic > local_apic { reinterpret_cast< decltype( local_apic ) >( ICS ) };
                Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ].printk(
                    DisplayColor::WHITE,
                    DisplayColor::BLACK,
                    "Local Apic ================>> | Acpi Processor UID <=> %8u | Apic ID <=> %27u | Flags <=> %41x |\n",
                    local_apic->acpiProcessorUID, local_apic->apicID, local_apic->flags );
            }
            else if ( ICS->type == ICSAttribute::I_O_APIC ) {
                Lib::Types::Ptr< IOApic > io_apic { reinterpret_cast< decltype( io_apic ) >( ICS ) };
                Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ].printk(
                    DisplayColor::WHITE,
                    DisplayColor::BLACK,
                    "I/O Apic ==================>> | I/O Apic ID <=> %15u | I/O Apic Address <=> %#18x | Global System Interrupt Base <=> %18x |\n",
                    io_apic->IOApicID, io_apic->IOApicAddress, io_apic->globalSystemInterruptBase );
                this->io_apic_address = reinterpret_cast< decltype( this->io_apic_address ) >( io_apic->IOApicAddress );
            }
            else if ( ICS->type == ICSAttribute::INTERRUPT_SOURCE_OVERRIDE ) {
                Lib::Types::Ptr< InterruptSourceOverride > ics { reinterpret_cast< decltype( ics ) >( ICS ) };
                Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ].printk(
                    DisplayColor::WHITE,
                    DisplayColor::BLACK,
                    "Interrupt Source Override =>> | Bus <=> %23u | Source <=> %28u | Flags <=> %41x | Global System Interrupt <=> %8u |\n",
                    ics->bus, ics->source, ics->flags, ics->globalSystemInterrupt );
            }
            length -= ICS->length;
            ICS = reinterpret_cast< decltype( ICS ) >( (char *)( ICS ) + ICS->length );
        }
        this->local_apic_address = reinterpret_cast< decltype( this->local_apic_address ) >( madt->localApicAddress );
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the advanced configuration power interface." << Lib::IO::endl;
    }

    template <>
    auto AcpiManagement::is_valid< Lib::Types::Rsdp >( VOID ) CONST->Lib::Types::BOOL {
        if ( Lib::STL::strncmp( this->rsdp->signature, "RSD PTR ", 8 ) || this->rsdp->revision != 2 ) {
            return FALSE;
        }
        if ( auto sum { bytes_sum_total( reinterpret_cast< Lib::Types::Ptr< CONST Lib::Types::uint8_t > >( this->rsdp ), 20 ) }; sum ) {
            return FALSE;
        }
        if ( auto sum { bytes_sum_total( reinterpret_cast< Lib::Types::Ptr< CONST Lib::Types::uint8_t > >( this->rsdp ), 36 ) }; sum ) {
            return FALSE;
        }
        return TRUE;
    }
}