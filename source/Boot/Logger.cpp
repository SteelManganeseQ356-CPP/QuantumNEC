#include <Boot/Include.hpp>
#include <Boot/Logger.hpp>
namespace QuantumNEC::Boot {
constexpr CONST auto levelCount { 4 };
auto GetLevelInfo( IN BootServiceLogger::LoggerLevel level ) -> CONST CHAR8 * {
    return "[INFO] ";
}
auto GetLevelError( IN BootServiceLogger::LoggerLevel level ) -> CONST CHAR8 * {
    return "[ERROR] ";
}
auto GetLevelSuccess( IN BootServiceLogger::LoggerLevel level ) -> CONST CHAR8 * {
    return "[SUCCESS] ";
}
auto GetLevelDebug( IN BootServiceLogger::LoggerLevel level ) -> CONST CHAR8 * {
    return "[DEBUG] ";
}
auto ( *GetLevel[ levelCount ] )( IN BootServiceLogger::LoggerLevel level ) -> CONST CHAR8 * {
    GetLevelInfo, GetLevelError, GetLevelSuccess, GetLevelDebug
};
auto BootServiceLogger::LogTip( IN LoggerLevel level, IN CONST CHAR8 *Message ) -> EFI_STATUS {
    CONST CHAR8 *StrLevel { GetLevel[ (UINT32)level ]( level ) };
    UINTN MsgLen { };
    EFI_STATUS Status { EFI_SUCCESS };
#ifdef PRINT_LOGGER
    AsciiPrint( "[" __DATE__ "<=>" __TIME__ "]" );
    AsciiPrint( StrLevel );
    AsciiPrint( Message );
    AsciiPrint( "\n\r" );     // 打印日志 ：[ 日期 <=> 时间 ] [Header] 消息
#endif
    MsgLen = AsciiStrLen( Message );
    Status = this->put( ).LogFile->Write( this->put( ).LogFile, &MsgLen,
                                          (VOID *)Message );     // 写入文件
    return Status;
}
auto BootServiceLogger::LogError( IN CONST EFI_STATUS Code ) -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    switch ( Code ) {
    case EFI_INVALID_PARAMETER:
        Status = this->LogTip( LoggerLevel::ERROR, "EFI_INVALID_PARAMETER.\n" );
        break;
    case EFI_NOT_FOUND:
        Status = this->LogTip( LoggerLevel::ERROR, "EFI_NOT_FOUND.\n" );
        break;
    case EFI_OUT_OF_RESOURCES:
        Status = this->LogTip( LoggerLevel::ERROR, "EFI_OUT_OF_RESOURCES.\n" );
        break;
    default:     // 最后一种情况
        Status = this->LogTip( LoggerLevel::ERROR, "Something must be wrong.\n" );
        break;
    }
    return Status;
}
auto BootServiceLogger::Close( VOID ) -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    Status = this->put( ).LogFile->Close( this->put( ).LogFile );
    return Status;
}
BootServiceLogger::BootServiceLogger( IN LoggerConfig *config ) :
    BootServiceDataManage< LoggerConfig > {
    config
}
{
    /* 初始化日志 */
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem { NULL };
    UINTN HandleCount { };
    EFI_HANDLE *Buffer { NULL };
    this->put( ).ErrorCode = gBS->LocateHandleBuffer( ByProtocol, &gEfiSimpleFileSystemProtocolGuid,
                                                      NULL, &HandleCount, &Buffer );
    this->put( ).ErrorCode = gBS->OpenProtocol(
        Buffer[ 0 ], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&FileSystem,
        this->putHandle( ), NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL );

    EFI_FILE_PROTOCOL *File { NULL };
    this->put( ).ErrorCode = FileSystem->OpenVolume( FileSystem, &File );

    this->put( ).ErrorCode = File->Open(
        File, &this->put( ).LogFile,
        (CHAR16 *)L"\\QuantumNEC\\Logger\\SystemLogger.log",
        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
        EFI_FILE_ARCHIVE );
    if ( EFI_ERROR( this->put( ).ErrorCode ) ) {
        AsciiPrint( "[" __DATE__ "<=>" __TIME__ "] [ERROR] Fail to read SystemLogger.log, Status: %r\n", this->put( ).ErrorCode );
    }
}
}     // namespace QuantumNEC::Boot