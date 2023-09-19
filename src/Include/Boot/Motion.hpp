#pragma once
#include <Boot/Data.hpp>
#include <Boot/File.hpp>
namespace QuantumNEC::Boot {
typedef struct
{
    UINTN Size;
    UINTN PageSize;
    UINTN Width;
    UINTN Height;
    UINTN Offset;
    UINT64 PixelStart;
    UINT64 BitsPerPx;
    auto
    set( VOID ) -> VOID {
    }
    auto
    put( VOID ) -> VOID {
    }
} BmpConfig;
typedef struct
{
    UINT8 sig[ 2 ];
    UINT32 file_size;
    UINT16 reserved1;
    UINT16 reserved2;
    UINT32 header_size;
    UINT32 info_header_size;
    UINT32 width;
    UINT32 height;
    UINT16 plane_num;
    UINT16 color_bit;
    UINT32 compression_type;
    UINT32 compression_size;
    UINT32 horizontal_pixel;
    UINT32 vertical_pixel;
    UINT32 color_num;
    UINT32 essentail_num;
} BmpHeader;
class BootServiceMotion : protected BootServiceDataManage< BmpConfig >
{
private:
    enum class BmpDisplayMode {
        MODE_NORMALl = 1,
        MODE_HOR_MIDDLE = ( 1 << 1 ),
        MODE_VER_MIDDLE = ( 1 << 2 ),
        MODE_TOP = ( 1 << 3 ),
        MODE_LEFT = ( 1 << 4 ),
        MODE_BOTTOM = ( 1 << 5 ),
        MODE_RIGHT = ( 1 << 6 ),
        MODE_CENTER = ( 1 << 7 )
    };

private:
    BootServiceFile fileUtils;

public:
    explicit BootServiceMotion( IN BmpConfig *config,
                                IN EFI_GRAPHICS_OUTPUT_PROTOCOL * );
    virtual ~BootServiceMotion( VOID ) = default;

public:
    auto bmpCheck( IN BmpHeader *bmpHdr ) -> decltype( auto );
    /**
     * @brief 使 bmp 格式成为可用于在 UEFI 上显示的像素。
     */
    auto bmpTranslate( IN EFI_PHYSICAL_ADDRESS BmpBase ) -> decltype( auto );

public:
    auto logoShow( IN wchar_t *logoPath ) -> EFI_STATUS;

private:
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop { };
};
}     // namespace QuantumNEC::Boot