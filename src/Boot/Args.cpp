#include <Boot/Args.hpp>
#include <Boot/Graphics.hpp>
#include <Boot/Include.hpp>
namespace QuantumNEC::Boot {
BootServiceArgs::BootServiceArgs( IN ArgsStackConfig *config,
                                  IN CONST UINT64 size ) :
    BootServiceDataManage< ArgsStackConfig > {
    config
}
{
    // 初始化config
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    UINT64 PageNumber { EFI_SIZE_TO_PAGES( size ) };
    this->put( ).pageNumber_ = PageNumber;
    this->put( ).data_ = AllocatePages( PageNumber );
    this->put( ).ptr_ = reinterpret_cast< UINT8 * >( this->put( ).data_ );
    displayStep( );
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the args stack management service." );
    logger.Close( );
}
auto BootServiceArgs::argsPush( IN OUT VOID **object, IN UINT64 size ) -> EFI_STATUS {
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "Push a data to args stack." );
    EFI_STATUS Status = EFI_SUCCESS;
    UINT8 *Ptr = reinterpret_cast< UINT8 * >( *object );
    UINT8 *WPtr = this->put( ).ptr_;
    while ( size-- && Ptr && WPtr ) {
        // 将数据弹入队列内
        *WPtr++ = *Ptr++;
        if ( reinterpret_cast< UINT64 >( this->put( ).data_ ) + EFI_PAGES_TO_SIZE( this->put( ).pageNumber_ ) - reinterpret_cast< UINT64 >( this->put( ).ptr_ ) == 0 ) {
            Status = EFI_OUT_OF_RESOURCES;
            logger.LogError( Status );
            logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Unable to push data - out of resources." );
            return Status;
        }
    }
    *object = reinterpret_cast< VOID * >( this->put( ).ptr_ );
    this->put( ).ptr_ = reinterpret_cast< UINT8 * >( ALIGN_POINTER( WPtr, 8 ) );
    logger.Close( );
    return Status;
}
}     // namespace QuantumNEC::Boot