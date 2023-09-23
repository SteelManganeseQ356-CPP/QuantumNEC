#include <Boot/File.hpp>
#include <Boot/Logger.hpp>
#include <Boot/Utils.hpp>
#include <Boot/Include.hpp>
namespace QuantumNEC::Boot {
BootServiceFile::BootServiceFile( VOID ) :
    BootServiceDataManage< FileConfig > {
    {}
}
{
}
auto BootServiceFile::GetFileHandle( IN wchar_t *FileName, OUT EFI_FILE_PROTOCOL **FileHandle ) -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    UINTN HandleCount { };
    EFI_HANDLE *HandleBuffer { };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    // 打开文件服务
    Status = gBS->LocateHandleBuffer( ByProtocol, &gEfiSimpleFileSystemProtocolGuid,
                                      NULL, &HandleCount, &HandleBuffer );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to LocateHanleBuffer of SimpleFileSystemProtocol." );
        logger.Close( );
        return Status;
    }
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Get handles that supported SimpleFileSystemProtocol." );
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem { };
    Status = gBS->OpenProtocol(
        HandleBuffer[ 0 ], &gEfiSimpleFileSystemProtocolGuid,
        reinterpret_cast< VOID ** >( &FileSystem ), this->putHandle( ), NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to open first handle that supported SimpleFileSystemProtocol." );
        logger.Close( );
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
auto BootServiceFile::ReadFile( IN EFI_FILE_PROTOCOL *File, OUT EFI_PHYSICAL_ADDRESS *FileBase ) -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    EFI_FILE_INFO *FileInfo { };
    UINTN InfoSize { sizeof( EFI_FILE_INFO ) + 128 };
    // 为文件信息描述表申请内存
    FileInfo = new EFI_FILE_INFO[ InfoSize ];

    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to AllocatePool for FileInfo." );
        logger.Close( );
        return Status;
    }
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Memory for FileInfo is ready." );
    // 从文件获取内容
    Status = File->GetInfo( File, &gEfiFileInfoGuid, &InfoSize, FileInfo );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to Get File Info." );
        logger.Close( );
        return Status;
    }
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Get File Info is ready." );
    // 为计算文件所占用的页数量并为其分配页
    UINTN FilePageSize { ( FileInfo->FileSize >> 12 ) + 1 };
    EFI_PHYSICAL_ADDRESS FileBufferAddress { };
    Status = gBS->AllocatePages( AllocateAnyPages, EfiLoaderData, FilePageSize,
                                 &FileBufferAddress );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to AllocatePages for File." );
        logger.Close( );
        return Status;
    }
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Memory for File is ready." );
    UINTN ReadSize { FileInfo->FileSize };
    // 读取内容
    Status = File->Read( File, &ReadSize, reinterpret_cast< VOID * >( FileBufferAddress ) );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to Read File." );
        logger.Close( );
        return Status;
    }
    delete[] FileInfo;
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "File is Reading." );
    *FileBase = FileBufferAddress;
    logger.Close( );
    return Status;
}
auto BootServiceFile::CloseFile( IN EFI_FILE_PROTOCOL *File ) -> EFI_STATUS {
    // 释放占用内存（关闭文件）
    return File->Close( File );
}
}     // namespace QuantumNEC::Boot