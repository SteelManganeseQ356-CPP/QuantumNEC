#include <Boot/Graphics.hpp>
#include <Boot/Include.hpp>
#include <Boot/Motion.hpp>
#include <Boot/Utils.hpp>
#include <Boot/Logger.hpp>
namespace QuantumNEC::Boot {
auto BootServiceMotion::bmpCheck( IN BmpHeader *bmpHdr ) -> decltype( auto ) {
}
auto BootServiceMotion::bmpTranslate( IN EFI_PHYSICAL_ADDRESS BmpBase ) -> decltype( auto ) {
    EFI_STATUS Status { EFI_SUCCESS };
    auto conversion { [ & ]( UINTN Offset, UINTN Size ) -> UINTN {
        UINTN Result = 0;
        for ( UINTN i = 0; i < Size; ++i ) {
            Result += *( reinterpret_cast< UINT8 * >( BmpBase ) + Offset + i ) << i * 8;
        }
        return Result;
    } };
    this->put( ).Size = conversion( 2, 4 );
    this->put( ).PageSize = ( this->put( ).Size >> 12 ) + 1;
    this->put( ).Offset = conversion( 10, 4 );
    this->put( ).Width = conversion( 18, 4 );
    this->put( ).Height = conversion( 22, 4 );
    EFI_PHYSICAL_ADDRESS PixelStart;
    Status = gBS->AllocatePages( AllocateAnyPages, EfiLoaderData,
                                 this->put( ).PageSize, &PixelStart );
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelFromFile = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)( BmpBase + this->put( ).Offset + this->put( ).Width * this->put( ).Height * 4 );
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelToBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)PixelStart;
    for ( UINTN i { }; i < this->put( ).Height; i++ ) {
        PixelFromFile -= this->put( ).Width;
        for ( UINTN j { }; j < this->put( ).Width; j++ ) {
            *PixelToBuffer = *PixelFromFile;
            PixelToBuffer++;
            PixelFromFile++;
        }
        PixelFromFile -= this->put( ).Width;
    }

    this->put( ).PixelStart = PixelStart;
    return Status;
}

auto BootServiceMotion::logoShow( IN wchar_t *logoPath ) -> EFI_STATUS {
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN Hor = this->Gop->Mode->Info->HorizontalResolution;
    UINTN Ver = this->Gop->Mode->Info->VerticalResolution;
    EFI_FILE_PROTOCOL *Logo;
    Status = this->fileUtils.GetFileHandle( logoPath, &Logo );

    EFI_PHYSICAL_ADDRESS LogoAddress;
    Status = this->fileUtils.ReadFile( Logo, &LogoAddress );

    Status = this->bmpTranslate( LogoAddress );

    UINTN X = ( Hor - this->put( ).Width ) / 2;
    UINTN Y = ( Ver - this->put( ).Height ) / 2;

    Status = Status = this->Gop->Blt(
        this->Gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)this->put( ).PixelStart,
        EfiBltBufferToVideo, 0, 0, X, Y, this->put( ).Width, this->put( ).Height,
        0 );

    return Status;
}
BootServiceMotion::BootServiceMotion( IN BmpConfig *config, IN EFI_GRAPHICS_OUTPUT_PROTOCOL *gop ) :
    BootServiceDataManage< BmpConfig > { config },
    Gop( gop ) {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the Motion service management service." );
    displayStep( );
    logger.Close( );
}
}     // namespace QuantumNEC::Boot