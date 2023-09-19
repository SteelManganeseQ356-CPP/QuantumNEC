#pragma once
#include <Boot/Acpi.hpp>
#include <Boot/Args.hpp>
#include <Boot/Data.hpp>
#include <Boot/ELF.hpp>
#include <Boot/Font.hpp>
#include <Boot/Graphics.hpp>
#include <Boot/Info.hpp>
#include <Boot/Logger.hpp>
#include <Boot/Memory.hpp>
#include <Boot/Motion.hpp>
namespace QuantumNEC::Boot {

typedef struct
{
    GraphicsConfig GraphicsData;
    MemoryConfig MemoryData;
    ArgsStackConfig ArgsData;
    AcpiConfig AcpiData;
    UnicodeTTF FontData;
    BmpConfig BmpData;
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} _packed BootConfig;
class BootServiceMain :
    public BootServiceGraphics,
    public BootServiceInfo,
    public BootServiceELF,
    public BootServiceMemory,
    public BootServicePage,
    public BootServiceArgs,
    public BootServiceAcpi,
    public BootServiceFont
{
public:
    /**
     * @brief 禁用计时器
     */
    auto closeTimer( VOID ) -> EFI_STATUS;
    /**
     * @brief 跳转内核
     */
    auto jumpToKernel( IN BootConfig * ) -> EFI_STATUS;

public:
    explicit BootServiceMain( IN BootConfig * );
    virtual ~BootServiceMain( VOID ) = default;
private:
    Config kernelConfig;
    PageConfig memoryPage;
};
}     // namespace QuantumNEC::Boot