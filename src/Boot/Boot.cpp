#include <Boot/Boot.hpp>
#include <Boot/Graphics.hpp>
#include <Boot/Include.hpp>
namespace QuantumNEC::Boot {
BootServiceMain::BootServiceMain( IN BootConfig *bootConfig ) :
    BootServiceGraphics { &bootConfig->GraphicsData },
    BootServiceInfo { &kernelConfig },
    BootServiceELF { },
    BootServiceMemory { &bootConfig->MemoryData },
    BootServicePage { &this->memoryPage },
    BootServiceArgs { &bootConfig->ArgsData, 0 },
    BootServiceAcpi { &bootConfig->AcpiData },
    BootServiceFont { &bootConfig->FontData } {
    // 为空指针赋值（一块地址）
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    displayStep( );
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the boot service" );
    logger.Close( );
}
auto BootServiceMain::closeTimer( VOID ) -> EFI_STATUS {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "Close the timer." );
    logger.Close( );
    // 禁用计时器
    return gBS->SetWatchdogTimer( 0, 0, 0, NULL );
}
auto BootServiceMain::jumpToKernel( IN BootConfig *config ) -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "Jump to kernel." );
    Status = this->closeTimer( );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to close the timer." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    // 全部装载到config
    config->GraphicsData = this->BootServiceGraphics::put( );
    config->MemoryData = this->BootServiceMemory::put( );
    config->ArgsData = this->BootServiceArgs::put( );
    config->AcpiData = this->BootServiceAcpi::put( );
    config->FontData = this->BootServiceFont::put( );
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "Exit the boot service." );
    // 退出启动时服务
    Status = gBS->ExitBootServices( this->BootServiceGraphics::putHandle( ), config->MemoryData.MemoryKey );
    // 跳转内核
    Status = reinterpret_cast< EFI_STATUS ( * )( IN BootConfig * ) >( this->address )( config );
    logger.Close( );

    return Status;
}

}     // namespace QuantumNEC::Boot