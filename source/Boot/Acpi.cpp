#include <Boot/acpi.hpp>
#include <Guid/Acpi.h>
#include <IndustryStandard/Acpi.h>
namespace QuantumNEC::Boot {
AcpiService::AcpiService( VOID ) noexcept {
    if ( EFI_ERROR( EfiGetSystemConfigurationTable( &gEfiAcpi20TableGuid, &this->acpi_table ) ) ) {
#ifdef DEBUG
        Print( L"[ERROR] Get System Configuration Table." );

#endif
        while ( TRUE )
            ;
    }
#ifdef DEBUG
    Print( L"[OK] Get System Configuration Table." );
#endif
}
AcpiService::~AcpiService( VOID ) noexcept {
#ifdef DEBUG
    Print( L"[EXIT] Exit Acpi Service." );
#endif
    this->acpi_table = NULL;
}
}     // namespace QuantumNEC::Boot