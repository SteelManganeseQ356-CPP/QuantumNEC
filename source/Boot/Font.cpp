#include <Boot/Font.hpp>
#include <Boot/Include.hpp>
#include <Boot/File.hpp>
#include <Boot/Graphics.hpp>
#include <Boot/Logger.hpp>
namespace QuantumNEC::Boot {
BootServiceFont::BootServiceFont( UnicodeTTF *config ) :
    BootServiceDataManager< UnicodeTTF > {
    config
}
{
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Initialize the font file management." );
    logger.Close( );
}

auto BootServiceFont::getUnicodeTTF( IN CONST wchar_t *fontPath ) -> EFI_STATUS {
    EFI_STATUS Status { EFI_SUCCESS };
    BootServiceFile fileUtils { };
    LoggerConfig logIni { };
    BootServiceLogger logger { &logIni };
    logger.LogTip( BootServiceLogger::LoggerLevel::INFO, "Start loading the font file." );
    // 读取Unicode TTF字体
    EFI_FILE_PROTOCOL *font { };
    Status = fileUtils.GetFileHandle( const_cast< wchar_t * >( fontPath ), &font );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to get font file handle." );
        logger.Close( );
        return Status;
    }
    EFI_PHYSICAL_ADDRESS fontBuffer { };
    Status = fileUtils.ReadFile( font, &fontBuffer );
    if ( EFI_ERROR( Status ) ) {
        logger.LogError( Status );
        logger.LogTip( BootServiceLogger::LoggerLevel::ERROR, "Failed to read font file." );
        logger.Close( );
        return Status;
    }
    this->put( ).fontBuffer = reinterpret_cast< VOID * >( fontBuffer );
    logger.LogTip( BootServiceLogger::LoggerLevel::SUCCESS, "Load the font file is OK." );
    logger.Close( );
    Status = displayStep( );
    return Status;
}
}     // namespace QuantumNEC::Boot