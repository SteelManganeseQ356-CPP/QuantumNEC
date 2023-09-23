#include <Boot/ELF.hpp>
#include <Boot/Graphics.hpp>
#include <Boot/Logger.hpp>
#include <Boot/Include.hpp>

namespace QuantumNEC::Boot {
BootServiceELF::BootServiceELF( VOID ) {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the ELF file service management." );
    displayStep( );
    logger.Close( );
}
auto BootServiceELF::loadKernel( IN CONST wchar_t *kernelPath ) -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    EFI_FILE_PROTOCOL *Kernel { };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "Loading the system kernel." );
    // 读取内核
    Status = this->fileUtils.GetFileHandle( const_cast< wchar_t * >( kernelPath ), &Kernel );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to get kernel handle." );
        logger.Close( );
        return Status;
    }
    EFI_PHYSICAL_ADDRESS KernelBuffer { };
    Status = this->fileUtils.ReadFile( Kernel, &KernelBuffer );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to read kernel." );
        logger.Close( );
        return Status;
    }
    // 解析内核是否为ELF格式
    Status = this->CheckELF( KernelBuffer );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "The format of the kernel isn't elf." );
        logger.Close( );
        return Status;
    }
    logger.Close( );
    // 装载内核
    Status = this->LoadSegments( KernelBuffer );
    return Status;
}

auto BootServiceELF::LoadSegments( IN EFI_PHYSICAL_ADDRESS KernelBufferBase )
    -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    ElfHeader64 *ElfHeader { reinterpret_cast< ElfHeader64 * >( KernelBufferBase ) };
    ProgramHeader64 *PHeader { reinterpret_cast< ProgramHeader64 * >( KernelBufferBase + ElfHeader->Phoff ) };
    EFI_PHYSICAL_ADDRESS LowAddr { 0xFFFFFFFFFFFFFFFF };
    EFI_PHYSICAL_ADDRESS HighAddr { };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    for ( UINTN i { }; i < ElfHeader->PHeadCount; ++i ) {
        if ( PHeader[ i ].Type == PT_LOAD ) {
            if ( LowAddr > PHeader[ i ].PAddress ) {
                LowAddr = PHeader[ i ].PAddress;
            }
            if ( HighAddr < ( PHeader[ i ].PAddress + PHeader[ i ].SizeInMemory ) ) {
                HighAddr = PHeader[ i ].PAddress + PHeader[ i ].SizeInMemory;
            }
        }
    }
    UINTN PageCount { ( ( HighAddr - LowAddr ) >> 12 ) + 1 };
    EFI_PHYSICAL_ADDRESS KernelRelocateBase { };
    Status = gBS->AllocatePages( AllocateAnyPages, EfiLoaderCode, PageCount, &KernelRelocateBase );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Allocate pages for kernelrelocatebuffer error." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    UINT64 RelocateOffset = KernelRelocateBase - LowAddr;
    UINT64 *ZeroStart = (UINT64 *)KernelRelocateBase;
    for ( UINTN i { }; i < ( PageCount << 9 ); i++ ) {
        *ZeroStart = 0x000000000000;
        ZeroStart++;
    }
    for ( UINTN i { }; i < ElfHeader->PHeadCount; i++ ) {
        if ( PHeader[ i ].Type == PT_LOAD ) {
            UINT8 *SourceStart { reinterpret_cast< UINT8 * >( KernelBufferBase + PHeader[ i ].Offset ) };
            UINT8 *DestStart { reinterpret_cast< UINT8 * >( PHeader[ i ].VAddress + RelocateOffset ) };
            for ( UINTN j { }; j < PHeader[ i ].SizeInFile; j++ ) {
                *DestStart = *SourceStart;
                ++SourceStart;
                ++DestStart;
            }
            // 获取内核页
        }
    }
    this->address = ElfHeader->Entry + RelocateOffset;

    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Loading the kernel." );
    logger.Close( );
    return Status;
}
auto BootServiceELF::CheckELF( IN EFI_PHYSICAL_ADDRESS KernelBuffer ) -> EFI_STATUS {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    EFI_STATUS Status { EFI_SUCCESS };
    // 检查文件头部是否包含ELF魔术字节并判断位数
    UINT32 Magic { };
    Magic += *( reinterpret_cast< UINT8 * >( KernelBuffer ) + 0 ) << 0 * 8;
    Magic += *( reinterpret_cast< UINT8 * >( KernelBuffer ) + 1 ) << 1 * 8;
    Magic += *( reinterpret_cast< UINT8 * >( KernelBuffer ) + 2 ) << 2 * 8;
    Magic += *( reinterpret_cast< UINT8 * >( KernelBuffer ) + 3 ) << 3 * 8;
    if ( Magic != ELF_MAGIC ) {
        Status = NOT_64_BIT;
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "The format of the file is elf_32" );
        return Status;
    }
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "The format of the file is elf_32" );
    return Status;
}
}     // namespace QuantumNEC::Boot
