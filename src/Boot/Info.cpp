#include <Boot/Graphics.hpp>
#include <Boot/Include.hpp>
#include <Boot/Info.hpp>
#include <Boot/Logger.hpp>
#include <Boot/Utils.hpp>
namespace QuantumNEC::Boot {
auto BootServiceInfo::iniParseLine( IN IniConfig *Ini, IN CHAR8 *Line ) -> decltype( auto ) {
}
auto BootServiceInfo::iniGet( IniConfig *ini, CHAR8 *key ) -> decltype( auto ) {
}
auto BootServiceInfo::iniLoad( IN wchar_t *Path ) -> decltype( auto ) {
    LoggerConfig LogIni { };
    BootServiceLogger logger { &LogIni };
    EFI_STATUS Status { EFI_SUCCESS };
    if ( !Path ) {
        logger.LogError( EFI_INVALID_PARAMETER );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Path is NULL." );
        logger.Close( );
        return EFI_INVALID_PARAMETER;
    }
    Status = this->fileUtils.GetFileHandle( Path, &this->file );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Get Config.ini file handle error." );
        logger.Close( );
        return Status;
    }
    Status = this->fileUtils.ReadFile( this->file, &this->iniBuffer );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Read Config.ini file error." );
        logger.Close( );
        return Status;
    }
    Status = this->fileUtils.CloseFile( this->file );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Close Config.ini file error." );
        logger.Close( );
        return Status;
    }
    logger.Close( );

    return Status;
}

auto BootServiceInfo::iniGetString( IN CHAR8 *Name ) -> wchar_t * {
    CHAR8 *IniRaw { reinterpret_cast< CHAR8 * >( this->iniBuffer ) };
    wchar_t *String { };
    UINT64 ComMentCount { };
    while ( *IniRaw++ ) {
        switch ( bool flags { true }; *IniRaw ) {
        case ':': {
            auto tmp1 { Name };
            auto tmp2 { IniRaw };
            while ( *tmp1++ ) {
                if ( !( *tmp1++ && *tmp1 == *tmp2++ ) ) {
                    flags = false;
                    break;
                }
                else {
                    flags = true;
                    continue;
                }
            }
            if ( !flags ) {
                break;
            }
            tmp2 = IniRaw;
            while ( *tmp2++ == '=' ) {
                while ( *tmp2 != '\n' ) {
                    ++ComMentCount;
                }
                String = reinterpret_cast< wchar_t * >(
                    AllocateZeroPool( sizeof( wchar_t ) * ( ComMentCount + 1 ) ) );
                for ( UINTN i { }; i < ComMentCount; ++i ) {
                    String[ i ] = static_cast< wchar_t >( *tmp2++ );
                }
            }
        }
            return String;
        case ';':
            break;
        default:
            break;
        }
    }
}
BootServiceInfo::BootServiceInfo( IN Config *config ) :
    BootServiceDataManage< Config > {
    config
}
{
    LoggerConfig LogIni { };
    BootServiceLogger logger { &LogIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the config system service management." );
    logger.Close( );
    displayStep( );
}
}     // namespace QuantumNEC::Boot