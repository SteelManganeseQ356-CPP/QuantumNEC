#pragma once
#include <Boot/Data.hpp>
#include <Boot/Logger.hpp>
namespace QuantumNEC::Boot {

// RSDP
typedef struct
{
    CHAR8 Signature[ 8 ];
    UINT8 Checksum;
    CHAR8 OEMID[ 6 ];
    UINT8 Revision;
    UINT32 RsdtAddress;
    UINT32 Length;
    UINT64 XsdtAddress;
    UINT8 ExtendedChecksum;
    CHAR8 Reserved[ 3 ];
} _packed Rsdp;
typedef struct
{
    Rsdp *rsdpTable;
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} AcpiConfig;

/**
 * @brief 引导时服务——Acpi
 */
class BootServiceAcpi :
    protected BootServiceDataManager< AcpiConfig >
{
public:
    explicit BootServiceAcpi( IN AcpiConfig *config );
    virtual ~BootServiceAcpi( VOID ) = default;

public:
    auto getApicTable( VOID ) -> EFI_STATUS;
};
}     // namespace QuantumNEC::Boot