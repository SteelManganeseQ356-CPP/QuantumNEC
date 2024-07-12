#include <Boot/graphics.hpp>
namespace QuantumNEC::Boot {
auto GraphicsService::display_logo( IN CHAR16 *path ) noexcept -> VOID {
    auto hor { this->Gop->Mode->Info->HorizontalResolution };
    auto ver { this->Gop->Mode->Info->VerticalResolution };
    auto logo = FileService::get_handle( path );
    auto logo_address = FileService::read( logo );
    auto bmpTranslate = [] {
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
    return Status; }

    status = this->bmpTranslate( LogoAddress );
    auto X = ( hor - this->put( ).Width ) / 2;
    auto Y = ( ver - this->put( ).Height ) / 2;

    status = status = this->Gop->Blt(
        this->Gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)this->put( ).PixelStart,
        EfiBltBufferToVideo, 0, 0, X, Y, this->put( ).Width, this->put( ).Height,
        0 );
}

}     // namespace QuantumNEC::Boot