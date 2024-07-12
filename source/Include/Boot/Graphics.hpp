#pragma once
#include <Boot/base.hpp>
#include <cstdint>
namespace QuantumNEC::Boot {
class GraphicsService
{
private:
    std::uint64_t FrameBufferBase;
    std::uint64_t FrameBufferSize;
    std::uint32_t HorizontalResolution;
    std::uint32_t VerticalResolution;
    std::uint32_t PixelsPerScanLine;

public:
    explicit GraphicsService( VOID ) noexcept;
    virtual ~GraphicsService( VOID ) noexcept;

public:
    auto display_logo( IN CHAR16 *path ) noexcept -> VOID;

private:
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
};

}     // namespace QuantumNEC::Boot