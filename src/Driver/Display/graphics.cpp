#include <Driver/Display/graphics.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Driver::Display {
    GraphicsManagement::GraphicsManagement(
        IN CONST Lib::Types::Ptr< Lib::Types::BootConfig > bootConfig ) :
        config( &bootConfig->GraphicsData ) {
    }
    auto GraphicsManagement::displayPixel(     // 填充一个像素
        IN Lib::Types::Ptr< Lib::Types::uint32_t > VideoHandle,
        IN Lib::Types::L_Ref< Lib::IO::DisplayColor > Color,
        IN Lib::Types::L_Ref< Lib::Types::int32_t > X,
        IN Lib::Types::L_Ref< Lib::Types::int32_t > Y,
        IN Lib::Types::L_Ref< Lib::Types::uint32_t > VideoLong )
        ->Lib::Types::SystemStatus {
        auto Pixel { VideoHandle };
        Pixel += Y * VideoLong + X;
        *Pixel = static_cast< Lib::Types::uint32_t >( Color );
        return SYSTEM_SUCCESS;
    }

    auto GraphicsManagement::SetPos(
        IN Lib::Types::L_Ref< Lib::Types::uint32_t > XResolution,
        IN Lib::Types::L_Ref< Lib::Types::uint32_t > YResolution,
        IN Lib::Types::int32_t XPosition, IN Lib::Types::int32_t YPosition,
        IN Lib::Types::int32_t XCharSize, IN Lib::Types::int32_t YCharSize,
        IN Lib::Types::int64_t column, IN Lib::Types::uint64_t row,
        IN Lib::Types::Ptr< Lib::Types::uint32_t > FB_addr,
        IN Lib::Types::uint64_t FB_length ) noexcept -> Lib::Types::SystemStatus {
        // 设置Pos结构体
        Lib::IO::Position Pos {
            .XResolution { static_cast< Lib::Types::int32_t >( XResolution ) },
            .YResolution { static_cast< Lib::Types::int32_t >( YResolution ) },
            .XPosition { XPosition },
            .YPosition { YPosition },
            .XCharSize { XCharSize },
            .YCharSize { YCharSize },
            .FB_addr { FB_addr },
            .FB_length { FB_length },
            .column { column },
            .row { row }
        };
        Lib::IO::sout.getPos( Pos );

        return SYSTEM_SUCCESS;
    }
    // 窗口初始化
    auto GraphicsManagement::windowInitialize( VOID )->Lib::Types::SystemStatus {
        Lib::Types::SystemStatus Status { SYSTEM_SUCCESS };
        Status = SetPos( this->config->HorizontalResolution,
                         this->config->VerticalResolution,
                         0,      // 开始的长度
                         0,      // 开始的高度
                         8,      // 字符长度
                         16,     // 字符高度
                         -1,     // \n列数
                         0,      // \n排数
                         reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint32_t > >(
                             this->config->FrameBufferBase ),
                         this->config->HorizontalResolution
                             * this->config->VerticalResolution * 4 );

        this->drawFill( 0, 0, this->config->HorizontalResolution,
                        this->config->VerticalResolution,
                        Lib::IO::DisplayColor::BLACK );
        return Status;
    }

    auto GraphicsManagement::GUIInitialize( VOID )->Lib::Types::SystemStatus {
        Lib::Types::SystemStatus Status { SYSTEM_SUCCESS };
        Status = SetPos( this->config->HorizontalResolution,
                         this->config->VerticalResolution,
                         1,      // 开始的长度
                         2,      // 开始的高度
                         8,      // 字符长度
                         16,     // 字符高度
                         1,      // \n列数
                         0,      // \n排数
                         reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint32_t > >(
                             this->config->FrameBufferBase ),
                         this->config->HorizontalResolution
                             * this->config->VerticalResolution * 4 );
        this->drawFill( 0, 0, this->config->HorizontalResolution,
                        this->config->VerticalResolution,
                        Lib::IO::DisplayColor::GRAYISH );
        return Status;
    }

    auto GraphicsManagement::displayShell( VOID )->Lib::Types::SystemStatus {
        Lib::Types::SystemStatus Status { SYSTEM_SUCCESS };

        // 第一层
        this->drawFill( 12, 12, 716, this->config->VerticalResolution, Lib::IO::DisplayColor::WHITE );
        // 第二层
        this->drawFill( 14, 28, 714, this->config->VerticalResolution - 4, Lib::IO::DisplayColor::GRAY );
        // 第三层
        this->drawFill( 16, 30, 712, this->config->VerticalResolution - 8, Lib::IO::DisplayColor::BLACK );
        return Status;
    }
    auto GraphicsManagement::errorBlueScreen(
        IN CONST Lib::Types::uint8_t exceptionNumber )
        ->Lib::Types::SystemStatus {
        if ( exceptionNumber >= 0x20 )
        {     // 如果是中断就没有必要画了
            return SYSTEM_SUCCESS;
        }
        Lib::Types::SystemStatus Status { SYSTEM_SUCCESS };
        Status = SetPos( this->config->HorizontalResolution,
                         this->config->VerticalResolution,
                         1,      // 开始的长度
                         1,      // 开始的高度
                         8,      // 字符长度
                         16,     // 字符高度
                         0,      // \n列数
                         0,      // \n排数
                         reinterpret_cast< Lib::Types::uint32_t far * >(
                             this->config->FrameBufferBase ),
                         this->config->HorizontalResolution
                             * this->config->VerticalResolution * 4 );

        // 填充背景
        this->drawFill( 0, 0, this->config->HorizontalResolution,
                        this->config->VerticalResolution,
                        Lib::IO::DisplayColor::WHITE );
        this->drawFill( 8, 8, this->config->HorizontalResolution - 8,
                        this->config->VerticalResolution - 8,
                        Lib::IO::DisplayColor::BLUE );
        return Status;
    }
    auto GraphicsManagement::drawFill(
        IN CONST Lib::Types::int32_t x0, IN CONST Lib::Types::int32_t y0,
        IN CONST Lib::Types::int32_t x1, IN CONST Lib::Types::int32_t y1,
        IN CONST Lib::Types::R_Ref< Lib::IO::DisplayColor > Color )
        ->Lib::Types::SystemStatus {
        Lib::Types::SystemStatus Status { SYSTEM_SUCCESS };
        auto DarkColor { Color };
        auto VideoHandle {
            reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint32_t > >(
                this->config->FrameBufferBase )
        };
        for ( auto y { y0 }; y < y1; ++y )
        // 按照坐标从x0 -> x1 y0 -> y1依次填充像素
        {
            for ( auto x { x0 }; x < x1; ++x )
            {
                Status = this->displayPixel( VideoHandle, DarkColor, x, y,
                                             this->config->HorizontalResolution );
            }
        }
        return Status;
    }
}