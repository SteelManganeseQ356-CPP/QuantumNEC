#pragma once
#include <Boot/Data.hpp>
#include <Protocol/SimpleFileSystem.h>
namespace QuantumNEC::Boot {
typedef struct
{
    EFI_STATUS ErrorCode { EFI_SUCCESS };
    EFI_FILE_PROTOCOL *LogFile { nullptr };
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} _packed LoggerConfig;

class BootServiceLogger : protected BootServiceDataManage< LoggerConfig >
{
public:
    enum class LoggerLevel : UINT32 {
        INFO = 0,
        ERROR = 1,
        SUCCESS = 2,
        DEBUG = 3
    };

public:
    /**
     * @brief 初始化日志
     * @param ImageHandle 屏幕参数
     */
    explicit BootServiceLogger( IN LoggerConfig * );
    virtual ~BootServiceLogger( VOID ) noexcept = default;

public:
    /**
     * @brief 保存一般提示信息
     * @param level 日志标签
     * @param Message 一般提示信息
     */
     auto LogTip( IN LoggerLevel level, IN CONST CHAR8 *Message ) -> EFI_STATUS;
    /**
     * @brief 根据错误码保存日志信息
     * @param Code 错误码
     */
     auto LogError( IN CONST EFI_STATUS Code ) -> EFI_STATUS;
    /**
     * @brief 关闭日志文件
     */
public:
    auto Close( VOID ) -> EFI_STATUS;
};
}     // namespace QuantumNEC::Boot