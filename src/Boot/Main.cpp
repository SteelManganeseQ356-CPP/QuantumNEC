#include <Boot/Boot.hpp>
#include <Boot/Include.hpp>
auto initializeGlobalData( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable ) -> VOID {     // 初始化全局变量
    QuantumNEC::Boot::GlobalImageHandle = ImageHandle;
    QuantumNEC::Boot::GlobalSystemTable = SystemTable;
    return;
}
extern "C" EFI_STATUS EFIAPI UefiMain( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable ) {
    EFI_STATUS Status { EFI_SUCCESS };
    initializeGlobalData( ImageHandle, SystemTable );
    QuantumNEC::Boot::BootConfig bootConfig { };
    QuantumNEC::Boot::BootServiceMain bootService { &bootConfig };
    // 读取Logo并显示
    Status = bootService.getBmpConfig( &bootConfig.BmpData ).displayLogo( L"\\EFI\\Boot\\Logo.BMP" );
    // 读取ACPI表
    Status = bootService.getApicTable( );
    // 获取内存布局
    Status = bootService.getMemoryMap( );
    // 读取并装载内核
    Status = bootService.loadKernel( L"\\QuantumNEC\\microKernel.elf" );
    // 设置内核页表
    Status = bootService.setPageTable( );
    // 读取Unicode字体并弹出
    Status = bootService.setUnicodeTTF( );
    // 跳转内核
    Status = bootService.jumpToKernel( &bootConfig );

    return EFI_SUCCESS;
}
