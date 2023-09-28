#include <Boot/Boot.hpp>
#include <Boot/Include.hpp>

extern "C" EFI_STATUS EFIAPI UefiMain( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable ) {
    using namespace QuantumNEC::Boot;
    EFI_STATUS Status { EFI_SUCCESS };
    initializeGlobalData( ImageHandle, SystemTable );
    BootConfig bootConfig { };
    BootServiceMain bootService { &bootConfig };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    // 读取Logo并显示
    Status = bootService.getBmpConfig( &bootConfig.BmpData ).displayLogo( L"\\EFI\\Boot\\Logo.BMP" );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to display logo." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    // 读取ACPI表
    Status = bootService.getApicTable( );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to get acpi table." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    // 获取内存布局
    Status = bootService.getMemoryMap( );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to get memory map." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    // 读取并装载内核
    Status = bootService.loadKernel( L"\\QuantumNEC\\microKernel.elf" );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to load kernel." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    // 设置内核页表
    Status = bootService.setPageTable( );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to set pages table." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    // 读取Unicode字体并弹出
    Status = bootService.getUnicodeTTF( L"\\QuantumNEC\\font\\Unicode.dll" );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to get Unicode Font file." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    // 跳转内核
    Status = bootService.jumpToKernel( &bootConfig );
    if ( EFI_ERROR( Status ) ) {
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to jump kernel." );
        logger.LogError( Status );
        logger.Close( );
        return Status;
    }
    return Status;
}
