#pragma once
#include <Boot/Data.hpp>
#include <Boot/Motion.hpp>
namespace QuantumNEC::Boot {
/**
 * @brief 绘制进度条的方块, 每次一格
 */
auto displayStep( VOID ) -> EFI_STATUS;

typedef struct
{
    EFI_PHYSICAL_ADDRESS FrameBufferBase;
    UINTN FrameBufferSize;
    UINT32 HorizontalResolution;
    UINT32 VerticalResolution;
    UINT32 PixelsPerScanLine;
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} GraphicsConfig;
class BootServiceGraphics : protected BootServiceDataManage< GraphicsConfig >
{
public:
    explicit BootServiceGraphics( IN GraphicsConfig * );
    virtual ~BootServiceGraphics( VOID ) = default;

private:
    /**
     * @brief 初始化图像
     * @param ImageHandle 屏幕参数
     * @param Gop 传入需要获取的Gop
     * @return EFI_STATUS
     */
    auto GetGopHandle( EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop ) -> decltype( auto );
    /**
     * @brief 设置显示模式
     * @param Gop 需要操作的Gop
     * @return EFI_STATUS
     */
    auto SetVideoMode( VOID ) -> decltype( auto );

public:
    auto getBmpConfig( IN BmpConfig *config ) -> BootServiceGraphics & {
        this->config_ = config;
        return *this;
    }

public:
    auto displayLogo( IN wchar_t *logoPath ) -> decltype( auto ) {
        BootServiceMotion bmpService { this->config_, this->Gop };
        bmpService.logoShow( logoPath );
        displayStep( );
        return EFI_SUCCESS;
    }

private:
    BmpConfig *config_ { };

private:
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
};
}     // namespace QuantumNEC::Boot