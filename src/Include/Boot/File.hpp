#pragma once
#include <Boot/Data.hpp>

namespace QuantumNEC::Boot {
constexpr CONST auto O_READ { EFI_FILE_MODE_READ };
constexpr CONST auto O_WRITE { EFI_FILE_MODE_WRITE };
constexpr CONST auto O_CREATE { EFI_FILE_MODE_CREATE };
constexpr CONST auto O_NAPEND { 0x4000000000000000ULL };     // 不追加,直接覆盖
typedef struct
{
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} FileConfig;
class BootServiceFile : protected BootServiceDataManage< FileConfig >
{
public:
    explicit BootServiceFile( VOID );
    virtual ~BootServiceFile( VOID ) = default;

public:
    /**
     * @brief 获取文件头
     * @param FileName 文件名
     * @param FileHandle 文件服务
     */
    auto GetFileHandle( IN wchar_t *FileName, OUT EFI_FILE_PROTOCOL **FileHandle )
        -> EFI_STATUS;
    /**
     * @brief 读取文件
     * @param File 文件服务
     * @param FileBase 存储地址
     */
    auto ReadFile( IN EFI_FILE_PROTOCOL *File, OUT EFI_PHYSICAL_ADDRESS *FileBase ) -> EFI_STATUS;

    /**
     * @brief 关闭文件
     * @brief File 文件服务
     */
    auto CloseFile( IN EFI_FILE_PROTOCOL *File ) -> EFI_STATUS;

private:
    EFI_FILE_PROTOCOL *gFP { NULL };
};
}     // namespace QuantumNEC::Boot