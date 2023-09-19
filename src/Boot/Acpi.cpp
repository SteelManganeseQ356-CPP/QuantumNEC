#include <Boot/Acpi.hpp>
#include <Boot/Include.hpp>
namespace QuantumNEC::Boot {
BootServiceAcpi::BootServiceAcpi( IN AcpiConfig *config ) :
    BootServiceDataManage< AcpiConfig > {
    config
}
{
    // 设置config
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the ACPI table management service. " );
    logger.Close( );
}
auto BootServiceAcpi::getApicTable( VOID ) -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };

    VOID *rsdpTable;
    // 挂载ACPI表
    Status = EfiGetSystemConfigurationTable( &gEfiAcpi20TableGuid, &rsdpTable );
    if ( EFI_ERROR( Status ) ) {
        logger.Close( );
        return Status;
    }
    this->put( ).rsdpTable = reinterpret_cast< RsdpConfig * >( rsdpTable );
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "Load the ACPI Table." );
    logger.Close( );
    return Status;
}
}     // namespace QuantumNEC::Boot