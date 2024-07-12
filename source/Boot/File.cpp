#include <Boot/file.hpp>
#include <Guid/FileInfo.h>
namespace QuantumNEC::Boot {
FileService::FileService( IN CHAR16 *path ) noexcept {
    std::uint64_t handle_count { };
    EFI_HANDLE *handle_buffer { };
    // 打开文件服务

    if ( EFI_ERROR( gBS->LocateHandleBuffer( ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &handle_count, &handle_buffer ) ) ) {
#ifdef DEBUG
        Print( L"[ERROR] Failed to LocateHanleBuffer of SimpleFileSystemProtocol." );
#endif
        while ( TRUE )
            ;
    }

#ifdef DEBUG
    Print( L"[OK] Get handles that supported SimpleFileSystemProtocol." );
#endif
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem { };

    if ( EFI_ERROR( gBS->OpenProtocol(
             handle_buffer[ 0 ], &gEfiSimpleFileSystemProtocolGuid,
             reinterpret_cast< VOID ** >( &FileSystem ), GlobalImageHandle, NULL,
             EFI_OPEN_PROTOCOL_GET_PROTOCOL ) ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to open first handle that supported SimpleFileSystemProtocol." );
        logger.Close( );
#ifdef DEBUG
        Print( L"[ERROR] Failed to open first handle that supported SimpleFileSystemProtocol." );
#endif
        return Status;
    }
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "SimpleFileSystemProtocol is opened with first handle." );
    EFI_FILE_PROTOCOL *Root { };
    // 获取文件头
    Status = FileSystem->OpenVolume( FileSystem, &Root );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to open volume." );
        logger.Close( );
        return Status;
    }
    // 读取这个文件到内存
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Volume is opened." );
    Status = Root->Open( Root, FileHandle, reinterpret_cast< CHAR16 * >( FileName ),
                         EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                         EFI_OPEN_PROTOCOL_GET_PROTOCOL );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to open file." );
        logger.Close( );
        return Status;
    }
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "File is opened." );
    // 关闭这个文件
    Status = Root->Close( Root );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to close file." );
        logger.Close( );
    }
    logger.Close( );
    return Status;
}

}     // namespace QuantumNEC::Boot