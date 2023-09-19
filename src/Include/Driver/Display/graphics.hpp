#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/IO/Base/base_print.hpp>
#include <Lib/Types/type_base.hpp>
#include <Lib/Types/type_bool.hpp>
#include <Lib/Types/type_int.hpp>
PUBLIC namespace QuantumNEC::Driver::Display {
    /**
     * @brief 图形化窗口驱动管理
     */
    PUBLIC class GraphicsManagement
    {
    public:
        explicit( TRUE ) GraphicsManagement( IN CONST Lib::Types::Ptr< Lib::Types::BootConfig > ) noexcept;
        virtual ~GraphicsManagement( VOID ) noexcept = default;

    public:
        /**
         * @brief 图形界面初始化
         */
        auto windowInitialize( VOID ) noexcept -> Lib::Types::SystemStatus;
        /**
         * @brief 桌面初始化
         */
        auto GUIInitialize( VOID ) noexcept -> Lib::Types::SystemStatus;
        /**
         * @brief 绘制终端
         */
        auto displayShell( VOID ) noexcept -> Lib::Types::SystemStatus;

    public:
        /**
         * @brief 蓝屏
         * @param exceptionNumber 异常中断号
         */
        auto errorBlueScreen( IN CONST Lib::Types::uint8_t exceptionNumber ) noexcept -> Lib::Types::SystemStatus;

    private:
        /**
         * @brief 绘制像素点
         * @param VideoHandle 要写入的地址
         * @param Color 写入像素点的颜色
         * @param X 横轴指标
         * @param Y 纵轴指标
         * @param VideoLong 视图长度
         */
        auto displayPixel( IN Lib::Types::Ptr< Lib::Types::uint32_t > VideoHandle,
                           IN Lib::Types::L_Ref< Lib::IO::DisplayColor > Color,
                           IN Lib::Types::L_Ref< Lib::Types::int32_t > X,
                           IN Lib::Types::L_Ref< Lib::Types::int32_t > Y,
                           IN Lib::Types::L_Ref< Lib::Types::uint32_t > VideoLong ) noexcept -> Lib::Types::SystemStatus;
        /**
         * @brief 设置Pos参数
         * @param XResolution 横轴长度
         * @param YResolution 纵轴高度
         * @param XPosition 开始打印的地址-横轴
         * @param YPosition 开始打印的地址-纵轴
         * @param XCharSize 字符长度
         * @param YCharSize 字符高度
         * @param column \n列数
         * @param row \n排数
         * @param FB_addr 起始地址
         * @param FB_length 字的长度
         */
        auto SetPos(
            IN Lib::Types::L_Ref< Lib::Types::uint32_t > XResolution,
            IN Lib::Types::L_Ref< Lib::Types::uint32_t > YResolution,
            IN Lib::Types::int32_t XPosition, IN Lib::Types::int32_t YPosition,
            IN Lib::Types::int32_t XCharSize, IN Lib::Types::int32_t YCharSize,
            IN Lib::Types::int64_t column, IN Lib::Types::uint64_t row,
            IN Lib::Types::Ptr< Lib::Types::uint32_t > FB_addr,
            IN Lib::Types::uint64_t FB_length ) noexcept -> Lib::Types::SystemStatus;

    public:
        /**
         * @brief 绘制方形
         * @param x0 左上角点坐标x
         * @param y0 左上角点坐标y
         * @param x1 右下角点坐标x
         * @param y1 右下角点坐标y
         * @param Color 填充颜色
         */
        auto drawFill(
            IN CONST Lib::Types::int32_t x0, IN CONST Lib::Types::int32_t y0,
            IN CONST Lib::Types::int32_t x1, IN CONST Lib::Types::int32_t y1,
            IN CONST Lib::Types::R_Ref< Lib::IO::DisplayColor > Color )
            -> Lib::Types::SystemStatus;
        ;

    private:
        Lib::Types::Ptr< Lib::Types::GraphicsConfig > config { };
    };
}