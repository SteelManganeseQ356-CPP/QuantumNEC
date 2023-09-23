#pragma once
#include <Boot/Data.hpp>
#include <Boot/File.hpp>
#define CONFIG_PATH L"\\EFI\\Boot\\Config.ini"
namespace QuantumNEC::Boot {
typedef struct KvTable
{
    CHAR8 *Key;
    CHAR8 *Value;
    KvTable *Next;
} _packed KvTable;

typedef struct
{
    UINT32 Count;
    KvTable *Kvs;
} _packed IniConfig;

typedef struct
{
    IniConfig ini;
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} _packed Config;

class BootServiceInfo : protected BootServiceDataManage< Config >
{
private:
    enum class InfoCur {
        KEY = 0x0,
        VALUE = 0x1,
        COMMENT = 0x2,
    };

public:
    explicit BootServiceInfo( IN Config * );
    virtual ~BootServiceInfo( VOID ) = default;

private:
    auto iniParseLine( IN IniConfig *, IN CHAR8 * ) -> decltype( auto );
    auto iniGet( IniConfig *ini, CHAR8 *key ) -> decltype( auto );
    auto iniLoad( IN wchar_t * ) -> decltype( auto );

public:
    auto configGetString( IN CHAR8 *Name ) -> decltype( auto ) {
        return this->iniGetString( Name );
    }

private:
    auto iniGetString( IN CHAR8 *Name ) -> wchar_t *;

private:
    EFI_PHYSICAL_ADDRESS iniBuffer { };

private:
    EFI_FILE_PROTOCOL *file { };

private:
    BootServiceFile fileUtils;
};
}     // namespace QuantumNEC::Boot