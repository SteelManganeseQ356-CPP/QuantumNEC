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
    UINT8 Reserved[ 3 ];
} RsdpConfig;
typedef struct
{
    CHAR8 Signature[ 4 ];
    UINT32 Length;
    UINT8 Reserved;
    UINT8 Checksum;
    CHAR8 OEMID[ 6 ];
    CHAR8 TableID[ 6 ];
    UINT32 OEMReserved;
    UINT32 CreatorID;
    UINT32 CreatorReserved;
} SDTHeader;
typedef struct
{
    SDTHeader Header;
    UINT32 LapicAddress;
    UINT32 Flags;
} _packed Madt;
typedef struct
{
    RsdpConfig *rsdpTable;
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} AcpiConfig;

/**
 * @brief 引导时服务——Acpi
 */
class BootServiceAcpi :
    protected BootServiceDataManage< AcpiConfig >
{
public:
    explicit BootServiceAcpi( IN AcpiConfig *config );
    virtual ~BootServiceAcpi( VOID ) = default;

public:
    auto getApicTable( VOID ) -> EFI_STATUS;
};
}     // namespace QuantumNEC::Boot