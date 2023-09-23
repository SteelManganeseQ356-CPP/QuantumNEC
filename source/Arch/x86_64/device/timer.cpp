#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Utils/asm.hpp>

PUBLIC namespace QuantumNEC::Architecture::Device {
    TimerDeviceManagement::TimerDeviceManagement( VOID ) {
        using namespace QuantumNEC::Lib::IO;
        sout[ ostream::HeadLevel::START ] << "Initialize the 8253 PIT." << endl;
        /* 设置 8253 的定时周期，也就是发中断的周期 */
        this->frequencySet( CONTRER0_PORT,
                            COUNTER0_NO,
                            READ_WRITE_LATCH,
                            COUNTER_MODE,
                            COUNTER0_VALUE );
        sout[ ostream::HeadLevel::OK ] << "Initialize the 8253 PIT." << endl;
    }
    TimerDeviceManagement::~TimerDeviceManagement( VOID ) {
    }
    auto TimerDeviceManagement::frequencySet(
        IN Lib::Types::uint8_t counter_port,
        IN Lib::Types::uint8_t counter_no,
        IN Lib::Types::uint8_t rwl,
        IN Lib::Types::uint8_t counter_mode,
        IN Lib::Types::uint16_t counter_value )
        ->VOID {
        /* 往控制字寄存器端口 0x43 中写入控制字 */
        Utils::io_out8( PIT_CONTROL_PORT, static_cast< Lib::Types::uint8_t >( counter_no << 6 | rwl << 4 | counter_mode << 1 ) );
        /* 先写入 counter_value 的低 8 位 */
        Utils::io_out8( counter_port, static_cast< Lib::Types::uint8_t >( counter_value ) );
        /* 再写入 counter_value 的高 8 位 */
        Utils::io_out8( counter_port, static_cast< Lib::Types::uint8_t >( counter_value ) >> 8 );
    }
}