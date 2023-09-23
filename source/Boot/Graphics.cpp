#include <Boot/Graphics.hpp>
#include <Boot/Include.hpp>
#include <Boot/Logger.hpp>
namespace QuantumNEC::Boot {
constexpr CONST auto GAP { 1 };
STATIC EFI_GRAPHICS_OUTPUT_BLT_PIXEL Grey = { 166, 166, 166, 0 };
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *GlobalGop { };
auto displayStep( VOID ) -> EFI_STATUS {
    STATIC UINT8 Step { 1 };
    // 把分辨率除以16
    UINTN BlockWidth { GlobalGop->Mode->Info->HorizontalResolution >> 6 };
    UINTN BlockHeight { GlobalGop->Mode->Info->VerticalResolution >> 6 };
    UINTN StartX { ( GlobalGop->Mode->Info->HorizontalResolution - ( BlockWidth + GAP ) * 10 - GAP ) / 2 };
    UINTN StartY { ( GlobalGop->Mode->Info->VerticalResolution * 3 ) >> 2 };
    UINTN X { StartX + ( BlockWidth + GAP ) * Step++ };
    // 画矩形
    return GlobalGop->Blt( GlobalGop, &Grey, EfiBltVideoFill, 0, 0, X, StartY, BlockWidth, BlockHeight, 0 );
}
auto BootServiceGraphics::SetVideoMode( VOID ) -> decltype( auto ) {
    EFI_STATUS Status { EFI_SUCCESS };
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo { };
    UINTN ModeInfoSize { sizeof( EFI_GRAPHICS_OUTPUT_MODE_INFORMATION ) };
    UINTN H { };
    UINTN V { };
    UINTN ModeIndex { };
    for ( UINTN i { }; i < this->Gop->Mode->MaxMode; ++i ) {
        Status = this->Gop->QueryMode( this->Gop, i, &ModeInfoSize, &ModeInfo );
        if ( EFI_ERROR( Status ) ) {
            return Status;
        }
        H = ModeInfo->HorizontalResolution;
        V = ModeInfo->VerticalResolution;
        if ( ( ( H == 1024 ) && ( V == 768 ) )     // 计算机所支持的分辨率
             || ( ( H == 1440 ) && ( V == 900 ) )
             || ( ( H == 1920 ) && ( V == 1080 ) ) ) {
            ModeIndex = i;
        }
    }
    Status = this->Gop->SetMode( this->Gop, ModeIndex );
    return Status;
}
BootServiceGraphics::BootServiceGraphics( IN GraphicsConfig *config ) :
    BootServiceDataManage< GraphicsConfig > {
    config
}
{
    // 初始化图形界面
    UINTN HandleCount { };
    EFI_HANDLE *HandleBuffer { };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    EFI_STATUS Status { EFI_SUCCESS };
    Status = gBS->LocateHandleBuffer( ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL,
                                      &HandleCount, &HandleBuffer );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to locate handle buffer for graphics." );
        logger.Close( );
    }
    Status = gBS->OpenProtocol( HandleBuffer[ 0 ], &gEfiGraphicsOutputProtocolGuid,     // 打开图形服务
                                reinterpret_cast< VOID ** >( &Gop ), this->putHandle( ),
                                NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to open graphics protocol." );
        logger.Close( );
    }
    Status = this->SetVideoMode( );     // 设置显示模式
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to set video mode." );
        logger.Close( );
    }
    this->put( ).FrameBufferBase = Gop->Mode->FrameBufferBase;
    this->put( ).FrameBufferSize = Gop->Mode->FrameBufferSize;
    this->put( ).HorizontalResolution = Gop->Mode->Info->HorizontalResolution;
    this->put( ).VerticalResolution = Gop->Mode->Info->VerticalResolution;
    this->put( ).PixelsPerScanLine = Gop->Mode->Info->PixelsPerScanLine;
    GlobalGop = Gop;
    Status = displayStep( );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to display step." );
        logger.Close( );
    }
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the graphics service management." );
    logger.Close( );
}
}     // namespace QuantumNEC::Boot