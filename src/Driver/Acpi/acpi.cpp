#include <Driver/Acpi/acpi.hpp>
#include <Lib/Debug/panic.hpp>
PUBLIC namespace QuantumNEC::Driver::Acpi {
    AcpiManagement::AcpiManagement( IN CONST Lib::Types::Ptr< Lib::Types::BootConfig > _config ) :
        rsdp { _config->AcpiData.rsdpTable },
        xsdt { reinterpret_cast< Lib::Types::Ptr< Lib::Types::XsdtConfig > >( this->rsdp->XsdtAddress ) } {
        using namespace Lib::Types;
        // 这仅包括此 Rsdp 的前 20 个字节，
        // 包括校验和字段。这些字节必须相加归零。
        // 注意：溢出为零。（有符号字符/无符号字符）
        if ( this->rsdp->Signature != RSDP_SIG || !this->checkSum( 20 ) )
            Lib::Debug::panic( "Rsdp is bad!" );

        if ( this->xsdt->Signature != XSDT_SIG || !this->checkSum( this->xsdt->Length ) )
            Lib::Debug::panic( "Xsdt is bad!" );
        Madt *madt = _EntryGet( APIC_SIG );
        if ( Madt == NULL )
            PANIC( "Can not find madt!!!" );

        int64 Len = Madt->Hdr.Length - sizeof( Madt_t );
        MadtIcs_t *Ics = OFFSET( Madt, sizeof( Madt_t ) );

        memset( _Gsi, 0xFF, 24 );

        while ( Len > 0 ) {
            switch ( Ics->Type ) {
            case ICS_LAPIC:
                // 写 `{}` 是明确变量的 life circle
                {
                    LApic_t *_LApic = (LApic_t *)Ics;
                    PrintK( "Local Apic -> Acpi Processor UID : %u\n"
                            "              Apic ID : %u\n"
                            "              Flags :   %x\n",
                            _LApic->AcpiProcessorUID, _LApic->ApicID, _LApic->Flgs );
                }
                break;

            case ICS_IOAPIC: {
                IOApic_t *_IOApic = (IOApic_t *)Ics;
                PrintK( "I/O Apic -> I/O Apic ID : %u\n"
                        "            I/O Apic Addr : %#x\n"
                        "            Global System Interrupt Base : %#x\n",
                        _IOApic->IOApicID, _IOApic->IOApicAddr, _IOApic->GsiBase );
                IOApic = (void *)(u64)_IOApic->IOApicAddr;
            } break;

            /* Interrupt Source Override */
            case ICS_ISO: {
                Iso_t *_Iso = (Iso_t *)Ics;
                PrintK( "Intr Src Override -> Bus : %u, Src : %u, Flgs : %x\n"
                        "                     Global sys intr : %u\n",
                        _Iso->Bus, _Iso->Src, _Iso->Flgs, _Iso->Gsi );
                _Gsi[ _Iso->Src ] = _Iso->Gsi;
            } break;
            }
            Len -= Ics->Length;
            Ics = OFFSET( Ics, Ics->Length );
        }

        LApic = (void *)(u64)Madt->LApicAddr;
    }
    auto AcpiManagement::checkSum( IN Lib::Types::size_t size )->Lib::Types::BOOL {
        Lib::Types::uint8_t sum { };
        for ( Lib::Types::size_t i { }; i < size; i++ ) {
            sum += ( reinterpret_cast< Lib::Types::Ptr< Lib::Types::char_t > >( this->rsdp ) )[ i ];
        }
        return !sum;
    }
    auto AcpiManagement::getEntry( IN Lib::Types::uint32_t sign )->Lib::Types::Ptr< VOID > {
        Lib::Types::size_t Num = ( this->xsdt.Length - sizeof( Xsdt_t ) ) / 8;

        Hdr_t *Ptr;
        Lib::Types::uint64_t *Addrs = (void *)__gXsdt + sizeof( Xsdt_t );

        for ( size_t i = 0; i < Num; i++ ) {
            Ptr = (Hdr_t *)( Addrs[ i ] );
            if ( Ptr->Sign == Sign ) {
                if ( _CheckSum( (void *)Ptr, Ptr->Length ) )
                    return (void *)Ptr;
            }
        }
        return NULL;
    }
}