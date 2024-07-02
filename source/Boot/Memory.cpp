#include <Boot/Memory.hpp>
#include <Boot/Include.hpp>
#include <Boot/Graphics.hpp>
#include <Boot/Logger.hpp>
#include <Boot/Utils.hpp>
namespace QuantumNEC::Boot {
BootServiceMemory::BootServiceMemory( IN MemoryConfig *config ) :
    BootServiceDataManager< MemoryConfig > {
        config
    } {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the memory service management." );
    logger.Close( );
}
auto BootServiceMemory::getMemoryMap( VOID ) -> EFI_STATUS {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    EFI_STATUS Status { EFI_SUCCESS };
    // 设置config
    this->put( ).Buffer = NULL;
    this->put( ).DescriptorSize = 0;
    this->put( ).DescriptorVersion = 0;
    this->put( ).MemoryKey = 0;
    this->put( ).MemoryCount = 0;
    this->put( ).MemorySize = 0;
    // 获取memory map
    while ( gBS->GetMemoryMap( &this->put( ).MemorySize, this->put( ).Buffer, &this->put( ).MemoryKey, &this->put( ).DescriptorSize, &this->put( ).DescriptorVersion ) == EFI_BUFFER_TOO_SMALL ) {
        if ( this->put( ).Buffer ) {
            delete this->put( ).Buffer;
            this->put( ).Buffer = NULL;
        }
        this->put( ).Buffer = new EFI_MEMORY_DESCRIPTOR[ this->put( ).MemorySize ];     // 为memory map分配内存块
        if ( !this->put( ).Buffer ) {
            logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to get Memory Map." );
            logger.LogError( EFI_INVALID_PARAMETER );
            logger.Close( );
            return EFI_INVALID_PARAMETER;
        }
    }
    // 获得内存块数量
    this->put( ).MemoryCount = this->put( ).MemorySize / this->put( ).DescriptorSize;
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Get Memory Map." );
    logger.Close( );
    Status = displayStep( );
    return Status;
}
}     // namespace QuantumNEC::Boot