#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Lib/STL/cstring>
#define ZERO "PUSHQ $0\n\t"
#define ERROR_CODE "NOP\n\t"
#define HANDLE_ENTRY( irq )                              \
    _C_LINK auto InterruptHandler##irq( VOID ) -> VOID { \
        ASM( "CLI\n\t"                                   \
             "PUSHQ $0\n\t"                              \
             "PUSHQ $" #irq "\n\t"                       \
             "LEAQ interrupt_entry(%RIP), %RAX\n\t"      \
             "JMP *%RAX" );                              \
    }
#define HANDLE_ENTRY_ERROR( irq )                        \
    _C_LINK auto InterruptHandler##irq( VOID ) -> VOID { \
        ASM( "CLI\n\t"                                   \
             "NOP\n\t"                                   \
             "PUSHQ $" #irq "\n\t"                       \
             "LEAQ interrupt_entry(%RIP), %RAX\n\t"      \
             "JMP *%RAX" );                              \
    }

using namespace QuantumNEC;
using namespace QuantumNEC::Architecture;
using namespace QuantumNEC::Lib::IO;
/**
 * @brief 中断入口函数表
 */
PUBLIC Architecture::CPU::InterruptDescriptorManagement::InterruptEntry interrupt_entry_table[ Platform::INTERRUPT_DESCRIPTOR_COUNT ] { };
PUBLIC Lib::Types::Ptr< CONST Lib::Types::char_t > interrupt_name[ Platform::INTERRUPT_DESCRIPTOR_COUNT ];     // 各个异常的名字

HANDLE_ENTRY( 0x00 );           //	#DE	除以0
HANDLE_ENTRY( 0x01 );           //	#DB	调试
HANDLE_ENTRY( 0x02 );           //	--- NMI中断
HANDLE_ENTRY( 0x03 );           //	#BP	断点
HANDLE_ENTRY( 0x04 );           //	#OF 溢出
HANDLE_ENTRY( 0x05 );           //	#BR 数组索引超限
HANDLE_ENTRY( 0x06 );           //	#UD	未定义指令
HANDLE_ENTRY( 0x07 );           //	#NM	设备未就绪
HANDLE_ENTRY_ERROR( 0x08 );     //    #DF	双重错误
HANDLE_ENTRY( 0x09 );           //    --- 协处理器错误
HANDLE_ENTRY_ERROR( 0x0a );     //    #TS	无效的TSS
HANDLE_ENTRY_ERROR( 0x0b );     //    #NP	段不存在
HANDLE_ENTRY_ERROR( 0x0c );     //    #SS 堆栈段故障
HANDLE_ENTRY_ERROR( 0x0d );     //    #GP 一般保护故障
HANDLE_ENTRY_ERROR( 0x0e );     //    #PF	缺页
HANDLE_ENTRY( 0x0f );           //    --- intel保留，请勿使用
HANDLE_ENTRY( 0x10 );           //    #MF	x87浮点数异常
HANDLE_ENTRY_ERROR( 0x11 );     //	#AC 内存数据引用未对齐 仅在Ring3触发
HANDLE_ENTRY( 0x12 );           //	#MC	处理器内部错误
HANDLE_ENTRY( 0x13 );           //    #XM	SIMD浮点数异常
HANDLE_ENTRY( 0x14 );           //    #CP 处理器内部错误
HANDLE_ENTRY_ERROR( 0x15 );     //    #HV 虚拟机注入的异常
HANDLE_ENTRY( 0x16 );           //    #VC VMM 通信失败
HANDLE_ENTRY( 0x17 );           //    #SX 安全异常
HANDLE_ENTRY( 0x18 );
HANDLE_ENTRY( 0x19 );
HANDLE_ENTRY( 0x1a );
HANDLE_ENTRY( 0x1b );
HANDLE_ENTRY( 0x1c );
HANDLE_ENTRY( 0x1d );
HANDLE_ENTRY( 0x1e );
HANDLE_ENTRY( 0x1f );
/*0x00-0x1f	保护模式异常(Intel保留)*/
HANDLE_ENTRY( 0x20 );     // 时钟中断对应的入口
HANDLE_ENTRY( 0x21 );     // 键盘中断对应的入口
HANDLE_ENTRY( 0x22 );     // 级联 (两个芯片内部使用。不会发起)
HANDLE_ENTRY( 0x23 );     // 串口COM2对应的入口 (如果开启)
HANDLE_ENTRY( 0x24 );     // 串口COM1对应的入口 (如果启用)
HANDLE_ENTRY( 0x25 );     // 并口LPT2对应的入口（如果启用）
HANDLE_ENTRY( 0x26 );     // 软盘对应的入口
HANDLE_ENTRY( 0x27 );     // 并口LPT1对应的入口 / 通常是“伪”中断（不可靠）
HANDLE_ENTRY( 0x28 );     // CMOS实时时钟(如果开启)
HANDLE_ENTRY( 0x29 );     // 自由外设/legacy SCSI/网卡
HANDLE_ENTRY( 0x2a );     // 自由外设/SCSI/网卡
HANDLE_ENTRY( 0x2b );     // 自由外设/SCSI/网卡
HANDLE_ENTRY( 0x2c );     // PS2鼠标
HANDLE_ENTRY( 0x2d );     // FPU/协处理器/Inter-processor
HANDLE_ENTRY( 0x2e );     // 主ATA硬盘
HANDLE_ENTRY( 0x2f );     // 辅ATA硬盘
HANDLE_ENTRY( 0x30 );
HANDLE_ENTRY( 0x31 );
HANDLE_ENTRY( 0x32 );
HANDLE_ENTRY( 0x33 );
HANDLE_ENTRY( 0x34 );
HANDLE_ENTRY( 0x35 );
HANDLE_ENTRY( 0x36 );
HANDLE_ENTRY( 0x37 );
HANDLE_ENTRY( 0x38 );
HANDLE_ENTRY( 0x39 );
HANDLE_ENTRY( 0x3a );
HANDLE_ENTRY( 0x3b );
HANDLE_ENTRY( 0x3c );
HANDLE_ENTRY( 0x3d );
HANDLE_ENTRY( 0x3e );
HANDLE_ENTRY( 0x3f );
HANDLE_ENTRY( 0x40 );
HANDLE_ENTRY( 0x41 );
HANDLE_ENTRY( 0x42 );
HANDLE_ENTRY( 0x43 );
HANDLE_ENTRY( 0x44 );
HANDLE_ENTRY( 0x45 );
HANDLE_ENTRY( 0x46 );
HANDLE_ENTRY( 0x47 );
HANDLE_ENTRY( 0x48 );
HANDLE_ENTRY( 0x49 );
HANDLE_ENTRY( 0x4a );
HANDLE_ENTRY( 0x4b );
HANDLE_ENTRY( 0x4c );
HANDLE_ENTRY( 0x4d );
HANDLE_ENTRY( 0x4e );
HANDLE_ENTRY( 0x4f );
HANDLE_ENTRY( 0x50 );
HANDLE_ENTRY( 0x51 );
HANDLE_ENTRY( 0x52 );
HANDLE_ENTRY( 0x53 );
HANDLE_ENTRY( 0x54 );
HANDLE_ENTRY( 0x55 );
HANDLE_ENTRY( 0x56 );
HANDLE_ENTRY( 0x57 );
HANDLE_ENTRY( 0x58 );
HANDLE_ENTRY( 0x59 );
HANDLE_ENTRY( 0x5a );
HANDLE_ENTRY( 0x5b );
HANDLE_ENTRY( 0x5c );
HANDLE_ENTRY( 0x5d );
HANDLE_ENTRY( 0x5e );
HANDLE_ENTRY( 0x5f );
HANDLE_ENTRY( 0x60 );
HANDLE_ENTRY( 0x61 );
HANDLE_ENTRY( 0x62 );
HANDLE_ENTRY( 0x63 );
HANDLE_ENTRY( 0x64 );
HANDLE_ENTRY( 0x65 );
HANDLE_ENTRY( 0x66 );
HANDLE_ENTRY( 0x67 );
HANDLE_ENTRY( 0x68 );
HANDLE_ENTRY( 0x69 );
HANDLE_ENTRY( 0x6a );
HANDLE_ENTRY( 0x6b );
HANDLE_ENTRY( 0x6c );
HANDLE_ENTRY( 0x6d );
HANDLE_ENTRY( 0x6e );
HANDLE_ENTRY( 0x6f );
HANDLE_ENTRY( 0x70 );
HANDLE_ENTRY( 0x71 );
HANDLE_ENTRY( 0x72 );
HANDLE_ENTRY( 0x73 );
HANDLE_ENTRY( 0x74 );
HANDLE_ENTRY( 0x75 );
HANDLE_ENTRY( 0x76 );
HANDLE_ENTRY( 0x77 );
HANDLE_ENTRY( 0x78 );
HANDLE_ENTRY( 0x79 );
HANDLE_ENTRY( 0x7a );
HANDLE_ENTRY( 0x7b );
HANDLE_ENTRY( 0x7c );
HANDLE_ENTRY( 0x7d );
HANDLE_ENTRY( 0x7e );
HANDLE_ENTRY( 0x7f );
HANDLE_ENTRY( 0x80 );     // 系统调用
HANDLE_ENTRY( 0x81 );
HANDLE_ENTRY( 0x82 );
HANDLE_ENTRY( 0x83 );
HANDLE_ENTRY( 0x84 );
HANDLE_ENTRY( 0x85 );
HANDLE_ENTRY( 0x86 );
HANDLE_ENTRY( 0x87 );
HANDLE_ENTRY( 0x88 );
HANDLE_ENTRY( 0x89 );
HANDLE_ENTRY( 0x8a );
HANDLE_ENTRY( 0x8b );
HANDLE_ENTRY( 0x8c );
HANDLE_ENTRY( 0x8d );
HANDLE_ENTRY( 0x8e );
HANDLE_ENTRY( 0x8f );
HANDLE_ENTRY( 0x90 );
HANDLE_ENTRY( 0x91 );
HANDLE_ENTRY( 0x92 );
HANDLE_ENTRY( 0x93 );
HANDLE_ENTRY( 0x94 );
HANDLE_ENTRY( 0x95 );
HANDLE_ENTRY( 0x96 );
HANDLE_ENTRY( 0x97 );
HANDLE_ENTRY( 0x98 );
HANDLE_ENTRY( 0x99 );
HANDLE_ENTRY( 0x9a );
HANDLE_ENTRY( 0x9b );
HANDLE_ENTRY( 0x9c );
HANDLE_ENTRY( 0x9d );
HANDLE_ENTRY( 0x9e );
HANDLE_ENTRY( 0x9f );
HANDLE_ENTRY( 0xa0 );
HANDLE_ENTRY( 0xa1 );
HANDLE_ENTRY( 0xa2 );
HANDLE_ENTRY( 0xa3 );
HANDLE_ENTRY( 0xa4 );
HANDLE_ENTRY( 0xa5 );
HANDLE_ENTRY( 0xa6 );
HANDLE_ENTRY( 0xa7 );
HANDLE_ENTRY( 0xa8 );
HANDLE_ENTRY( 0xa9 );
HANDLE_ENTRY( 0xaa );
HANDLE_ENTRY( 0xab );
HANDLE_ENTRY( 0xac );
HANDLE_ENTRY( 0xad );
HANDLE_ENTRY( 0xae );
HANDLE_ENTRY( 0xaf );
HANDLE_ENTRY( 0xb0 );
HANDLE_ENTRY( 0xb1 );
HANDLE_ENTRY( 0xb2 );
HANDLE_ENTRY( 0xb3 );
HANDLE_ENTRY( 0xb4 );
HANDLE_ENTRY( 0xb5 );
HANDLE_ENTRY( 0xb6 );
HANDLE_ENTRY( 0xb7 );
HANDLE_ENTRY( 0xb8 );
HANDLE_ENTRY( 0xb9 );
HANDLE_ENTRY( 0xba );
HANDLE_ENTRY( 0xbb );
HANDLE_ENTRY( 0xbc );
HANDLE_ENTRY( 0xbd );
HANDLE_ENTRY( 0xbe );
HANDLE_ENTRY( 0xbf );
HANDLE_ENTRY( 0xc0 );
HANDLE_ENTRY( 0xc1 );
HANDLE_ENTRY( 0xc2 );
HANDLE_ENTRY( 0xc3 );
HANDLE_ENTRY( 0xc4 );
HANDLE_ENTRY( 0xc5 );
HANDLE_ENTRY( 0xc6 );
HANDLE_ENTRY( 0xc7 );
HANDLE_ENTRY( 0xc8 );
HANDLE_ENTRY( 0xc9 );
HANDLE_ENTRY( 0xca );
HANDLE_ENTRY( 0xcb );
HANDLE_ENTRY( 0xcc );
HANDLE_ENTRY( 0xcd );
HANDLE_ENTRY( 0xce );
HANDLE_ENTRY( 0xcf );
HANDLE_ENTRY( 0xd0 );
HANDLE_ENTRY( 0xd1 );
HANDLE_ENTRY( 0xd2 );
HANDLE_ENTRY( 0xd3 );
HANDLE_ENTRY( 0xd4 );
HANDLE_ENTRY( 0xd5 );
HANDLE_ENTRY( 0xd6 );
HANDLE_ENTRY( 0xd7 );
HANDLE_ENTRY( 0xd8 );
HANDLE_ENTRY( 0xd9 );
HANDLE_ENTRY( 0xda );
HANDLE_ENTRY( 0xdb );
HANDLE_ENTRY( 0xdc );
HANDLE_ENTRY( 0xdd );
HANDLE_ENTRY( 0xde );
HANDLE_ENTRY( 0xdf );
HANDLE_ENTRY( 0xe0 );
HANDLE_ENTRY( 0xe1 );
HANDLE_ENTRY( 0xe2 );
HANDLE_ENTRY( 0xe3 );
HANDLE_ENTRY( 0xe4 );
HANDLE_ENTRY( 0xe5 );
HANDLE_ENTRY( 0xe6 );
HANDLE_ENTRY( 0xe7 );
HANDLE_ENTRY( 0xe8 );
HANDLE_ENTRY( 0xe9 );
HANDLE_ENTRY( 0xea );
HANDLE_ENTRY( 0xeb );
HANDLE_ENTRY( 0xec );
HANDLE_ENTRY( 0xed );
HANDLE_ENTRY( 0xee );
HANDLE_ENTRY( 0xef );
HANDLE_ENTRY( 0xf0 );
HANDLE_ENTRY( 0xf1 );
HANDLE_ENTRY( 0xf2 );
HANDLE_ENTRY( 0xf3 );
HANDLE_ENTRY( 0xf4 );
HANDLE_ENTRY( 0xf5 );
HANDLE_ENTRY( 0xf6 );
HANDLE_ENTRY( 0xf7 );
HANDLE_ENTRY( 0xf8 );
HANDLE_ENTRY( 0xf9 );
HANDLE_ENTRY( 0xfa );
HANDLE_ENTRY( 0xfb );
HANDLE_ENTRY( 0xfc );
HANDLE_ENTRY( 0xfd );
HANDLE_ENTRY( 0xfe );
HANDLE_ENTRY( 0xff );
#undef HANDLE_ENTRY
_C_LINK ASMCALL auto general_interrupt_handler( IN Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > frame ) -> Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > {
#ifndef APIC
    if ( frame->irq == 0x27 || frame->irq == 0x2f )     // 0x2f是从片8259A上的最后一个irq引脚，保留
        return frame;                                   // IRQ7和IRQ15会产生伪中断(spurious interrupt),无须处理。
#endif
    if ( interrupt_entry_table[ frame->irq ] == general_interrupt_handler ) {
        // 显示中断名
        sout[ ostream::HeadLevel::ERROR ] << "IRQ_" << frame->irq << ":" << ::interrupt_name[ frame->irq ] << endl;
        // 显示各个寄存器值
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "Rflags -> %#018lx\n\r", frame->rflags );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RIP -> %#018lx\tRSP -> %#018lx\tSS  -> %#018lx\tCS  -> %#018lx\n\r", frame->rip, frame->rsp, frame->ss, frame->cs );
        Architecture::CPU::InterruptDescriptorManagement::display_registers( &frame->regs );
        if ( frame->error_code ) {
            sout[ ostream::HeadLevel::INFO ] << "Error Code -> " << frame->error_code << endl;
        }
        /* #TS #NP #SS #GP特殊处理*/
        if ( frame->irq == 0x0a || frame->irq == 0x0b || frame->irq == 0x0c || frame->irq == 0x0d ) {
            if ( frame->error_code & 0x01 ) {
                sout[ ostream::HeadLevel::INFO ] << "The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception." << endl;
            }
            if ( frame->error_code & 0x02 ) {
                sout[ ostream::HeadLevel::INFO ] << "Refers to a gate descriptor in the IDT." << endl;
            }
            else {
                sout[ ostream::HeadLevel::INFO ] << "Refers to a descriptor in the GDT or the current LDT." << endl;
            }
            if ( !( frame->error_code & 0x02 ) ) {
                if ( frame->error_code & 0x04 ) {
                    sout[ ostream::HeadLevel::INFO ] << "Refers to a segment or gate descriptor in the LDT." << endl;
                }
                else {
                    sout[ ostream::HeadLevel::INFO ] << "Refers to a descriptor in the current GDT." << endl;
                }
            }
            sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "Segment Selector Index : %#010x\n", frame->error_code & 0xfff8 );
        }

        if ( frame->irq == 0x0e ) /* #PF的特殊处理 */
        {
            sout[ ostream::HeadLevel::INFO ];
            if ( !( frame->error_code & 0x01 ) ) {
                sout << "Page Not-Present, ";
            }
            if ( frame->error_code & 0x02 ) {
                sout << "Write Cause Fault, ";
            }
            else {
                sout << "Read Cause Fault, ";
            }
            if ( frame->error_code & 0x04 ) {
                sout << "Fault in user(3), ";
            }
            else {
                sout << "Fault in supervisor(0,1,2), ";
            }
            if ( frame->error_code & 0x08 ) {
                sout << "Reserved Bit Cause Fault, ";
            }
            if ( frame->error_code & 0x10 ) {
                sout << "Instruction fetch Cause Fault";
            }
            endl( sout );
        }
        while ( TRUE )     // 因为进入异常处理时已经关闭了中断所以直接阻塞即可
            Architecture::CPU::CPUManagement::hlt( );
    }
    else {
        return interrupt_entry_table[ frame->irq ]( frame );
    }
}
/**
 * @brief 定义异常接收函数
 */

/**
 * @brief 设置中断入口（Trap）
 */
#define SET_TRAP_HANDLER( index, ist )                       \
    this->make_descriptor(                                   \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        Platform::SELECTOR_CODE64_KERNEL,                    \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::TRAP ) )
/**
 * @brief 设置中断入口（System）
 */
#define SET_SYSTEM_HANDLER( index, ist )                     \
    this->make_descriptor(                                   \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        Platform::SELECTOR_CODE64_KERNEL,                    \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::SYSTEM ) )
/**
 * @brief 设置中断入口（System——Interrupt）
 */
#define SET_SYSTEM_INTERRUPT_HANDLER( index, ist )           \
    this->make_descriptor(                                   \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        Platform::SELECTOR_CODE64_KERNEL,                    \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::SYSTEM_INTERRUPT ) )
/**
 * @brief 设置中断入口（Interrupt）
 */
#define SET_INTERRUPT_HANDLER( index, ist )                  \
    this->make_descriptor(                                   \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        Platform::SELECTOR_CODE64_KERNEL,                    \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::INTERRUPT ) )
PUBLIC namespace QuantumNEC::Architecture::CPU {
    InterruptDescriptorManagement::InterruptDescriptorManagement( IN Lib::Types::Ptr< InterruptDescriptor > _descriptor, IN Lib::Types::uint16_t _num ) noexcept :
        Platform::DescriptorManagement< InterruptDescriptor > {
            _descriptor, _num
        } {
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the interrupt descriptor table management." << Lib::IO::endl;
        Architecture::CPU::CPUManagement::cli( );     // 关中断
        Lib::STL::memset( this->xdtr.descriptor, 0, Platform::INTERRUPT_DESCRIPTOR_COUNT * sizeof( InterruptDescriptor ) );
        for ( Lib::Types::uint16_t i { }; i < Platform::INTERRUPT_DESCRIPTOR_COUNT; ++i ) {
            interrupt_name[ i ] = "unknown";                            // 先统一赋值为unknown
            interrupt_entry_table[ i ] = general_interrupt_handler;     // 默认为general_interrupt_handler
        }
        interrupt_name[ 0 ] = "#DE Divide-by-zero Error";
        interrupt_name[ 1 ] = "#DB Debug";
        interrupt_name[ 2 ] = "--- Non Maskable Interrupt(NMI)";
        interrupt_name[ 3 ] = "#BP Breakpoint";
        interrupt_name[ 4 ] = "#OF Overflow";
        interrupt_name[ 5 ] = "#BR Bound Range Exceeded Exception";
        interrupt_name[ 6 ] = "#UD Invalid Opcode Exception";
        interrupt_name[ 7 ] = "#NM Device Not Available Exception";
        interrupt_name[ 8 ] = "#DF Double Fault Exception";
        interrupt_name[ 9 ] = "--- Coprocessor Segment Overrun";
        interrupt_name[ 10 ] = "#TS Invalid TSS Exception";
        interrupt_name[ 11 ] = "#NP Segment Not Present";
        interrupt_name[ 12 ] = "#SS Stack-Segment Fault Exception";
        interrupt_name[ 13 ] = "#GP General Protection Exception";
        interrupt_name[ 14 ] = "#PF Page Fault Exception";
        interrupt_name[ 15 ] = "--- Intel Reserved";     // 第15项是intel保留项，未使用
        interrupt_name[ 16 ] = "#MF x87 Floating-Point Exception";
        interrupt_name[ 17 ] = "#AC Alignment Check";
        interrupt_name[ 18 ] = "#MC Machine Check Exception";
        interrupt_name[ 19 ] = "#XF SIMD Floating-Point Exception";
        interrupt_name[ 20 ] = "#CP Control Protection Exception";
        for ( Lib::Types::uint16_t i { 21 }; i < 27; ++i ) {
            interrupt_name[ i ] = "--- System Reserved";
        }
        interrupt_name[ 27 ] = "#HV Exception Injected By The Virtual Machine";
        interrupt_name[ 28 ] = "#VC VMM Communication Failed";
        interrupt_name[ 29 ] = "#SX Security Exception";

        /* 注册所有入口函数到中断描述符表 */
        sout[ ostream::HeadLevel::INFO ] << "Setting the interrupt handler entry for the interrupt descriptor table" << endl;

        SET_TRAP_HANDLER( 0x00, 0 );
        SET_TRAP_HANDLER( 0x01, 0 );
        SET_INTERRUPT_HANDLER( 0x02, 0 );
        SET_SYSTEM_HANDLER( 0x03, 0 );
        SET_SYSTEM_HANDLER( 0x04, 0 );
        SET_SYSTEM_HANDLER( 0x05, 0 );
        SET_TRAP_HANDLER( 0x06, 0 );
        SET_TRAP_HANDLER( 0x07, 0 );
        SET_TRAP_HANDLER( 0x08, 0 );
        SET_TRAP_HANDLER( 0x09, 0 );
        SET_TRAP_HANDLER( 0x0a, 0 );
        SET_TRAP_HANDLER( 0x0b, 0 );
        SET_TRAP_HANDLER( 0x0c, 0 );
        SET_TRAP_HANDLER( 0x0d, 0 );
        SET_TRAP_HANDLER( 0x0e, 0 );
        SET_TRAP_HANDLER( 0x0f, 0 );
        SET_TRAP_HANDLER( 0x10, 0 );
        SET_TRAP_HANDLER( 0x11, 0 );
        SET_TRAP_HANDLER( 0x12, 0 );
        SET_TRAP_HANDLER( 0x13, 0 );
        SET_TRAP_HANDLER( 0x14, 0 );
        SET_TRAP_HANDLER( 0x15, 0 );
        SET_TRAP_HANDLER( 0x16, 0 );
        SET_TRAP_HANDLER( 0x17, 0 );
        SET_TRAP_HANDLER( 0x18, 0 );
        SET_TRAP_HANDLER( 0x19, 0 );
        SET_TRAP_HANDLER( 0x1a, 0 );
        SET_TRAP_HANDLER( 0x1b, 0 );
        SET_TRAP_HANDLER( 0x1c, 0 );
        SET_TRAP_HANDLER( 0x1d, 0 );
        SET_TRAP_HANDLER( 0x1e, 0 );
        SET_TRAP_HANDLER( 0x1f, 0 );
        SET_INTERRUPT_HANDLER( 0x20, 0 );
        SET_INTERRUPT_HANDLER( 0x21, 0 );
        SET_INTERRUPT_HANDLER( 0x22, 0 );
        SET_INTERRUPT_HANDLER( 0x23, 0 );
        SET_INTERRUPT_HANDLER( 0x24, 0 );
        SET_INTERRUPT_HANDLER( 0x25, 0 );
        SET_INTERRUPT_HANDLER( 0x26, 0 );
        SET_INTERRUPT_HANDLER( 0x27, 0 );
        SET_INTERRUPT_HANDLER( 0x28, 0 );
        SET_INTERRUPT_HANDLER( 0x29, 0 );
        SET_INTERRUPT_HANDLER( 0x2a, 0 );
        SET_INTERRUPT_HANDLER( 0x2b, 0 );
        SET_INTERRUPT_HANDLER( 0x2c, 0 );
        SET_INTERRUPT_HANDLER( 0x2d, 0 );
        SET_INTERRUPT_HANDLER( 0x2e, 0 );
        SET_INTERRUPT_HANDLER( 0x2f, 0 );
        SET_INTERRUPT_HANDLER( 0x30, 0 );
        SET_INTERRUPT_HANDLER( 0x31, 0 );
        SET_INTERRUPT_HANDLER( 0x32, 0 );
        SET_INTERRUPT_HANDLER( 0x33, 0 );
        SET_INTERRUPT_HANDLER( 0x34, 0 );
        SET_INTERRUPT_HANDLER( 0x35, 0 );
        SET_INTERRUPT_HANDLER( 0x36, 0 );
        SET_INTERRUPT_HANDLER( 0x37, 0 );
        SET_INTERRUPT_HANDLER( 0x38, 0 );
        SET_INTERRUPT_HANDLER( 0x39, 0 );
        SET_INTERRUPT_HANDLER( 0x3a, 0 );
        SET_INTERRUPT_HANDLER( 0x3b, 0 );
        SET_INTERRUPT_HANDLER( 0x3c, 0 );
        SET_INTERRUPT_HANDLER( 0x3d, 0 );
        SET_INTERRUPT_HANDLER( 0x3e, 0 );
        SET_INTERRUPT_HANDLER( 0x3f, 0 );
        SET_INTERRUPT_HANDLER( 0x40, 0 );
        SET_INTERRUPT_HANDLER( 0x41, 0 );
        SET_INTERRUPT_HANDLER( 0x42, 0 );
        SET_INTERRUPT_HANDLER( 0x43, 0 );
        SET_INTERRUPT_HANDLER( 0x44, 0 );
        SET_INTERRUPT_HANDLER( 0x45, 0 );
        SET_INTERRUPT_HANDLER( 0x46, 0 );
        SET_INTERRUPT_HANDLER( 0x47, 0 );
        SET_INTERRUPT_HANDLER( 0x48, 0 );
        SET_INTERRUPT_HANDLER( 0x49, 0 );
        SET_INTERRUPT_HANDLER( 0x4a, 0 );
        SET_INTERRUPT_HANDLER( 0x4b, 0 );
        SET_INTERRUPT_HANDLER( 0x4c, 0 );
        SET_INTERRUPT_HANDLER( 0x4d, 0 );
        SET_INTERRUPT_HANDLER( 0x4e, 0 );
        SET_INTERRUPT_HANDLER( 0x4f, 0 );
        SET_INTERRUPT_HANDLER( 0x50, 0 );
        SET_INTERRUPT_HANDLER( 0x51, 0 );
        SET_INTERRUPT_HANDLER( 0x52, 0 );
        SET_INTERRUPT_HANDLER( 0x53, 0 );
        SET_INTERRUPT_HANDLER( 0x54, 0 );
        SET_INTERRUPT_HANDLER( 0x55, 0 );
        SET_INTERRUPT_HANDLER( 0x56, 0 );
        SET_INTERRUPT_HANDLER( 0x57, 0 );
        SET_INTERRUPT_HANDLER( 0x58, 0 );
        SET_INTERRUPT_HANDLER( 0x59, 0 );
        SET_INTERRUPT_HANDLER( 0x5a, 0 );
        SET_INTERRUPT_HANDLER( 0x5b, 0 );
        SET_INTERRUPT_HANDLER( 0x5c, 0 );
        SET_INTERRUPT_HANDLER( 0x5d, 0 );
        SET_INTERRUPT_HANDLER( 0x5e, 0 );
        SET_INTERRUPT_HANDLER( 0x5f, 0 );
        SET_INTERRUPT_HANDLER( 0x60, 0 );
        SET_INTERRUPT_HANDLER( 0x61, 0 );
        SET_INTERRUPT_HANDLER( 0x62, 0 );
        SET_INTERRUPT_HANDLER( 0x63, 0 );
        SET_INTERRUPT_HANDLER( 0x64, 0 );
        SET_INTERRUPT_HANDLER( 0x65, 0 );
        SET_INTERRUPT_HANDLER( 0x66, 0 );
        SET_INTERRUPT_HANDLER( 0x67, 0 );
        SET_INTERRUPT_HANDLER( 0x68, 0 );
        SET_INTERRUPT_HANDLER( 0x69, 0 );
        SET_INTERRUPT_HANDLER( 0x6a, 0 );
        SET_INTERRUPT_HANDLER( 0x6b, 0 );
        SET_INTERRUPT_HANDLER( 0x6c, 0 );
        SET_INTERRUPT_HANDLER( 0x6d, 0 );
        SET_INTERRUPT_HANDLER( 0x6e, 0 );
        SET_INTERRUPT_HANDLER( 0x6f, 0 );
        SET_INTERRUPT_HANDLER( 0x70, 0 );
        SET_INTERRUPT_HANDLER( 0x71, 0 );
        SET_INTERRUPT_HANDLER( 0x72, 0 );
        SET_INTERRUPT_HANDLER( 0x73, 0 );
        SET_INTERRUPT_HANDLER( 0x74, 0 );
        SET_INTERRUPT_HANDLER( 0x75, 0 );
        SET_INTERRUPT_HANDLER( 0x76, 0 );
        SET_INTERRUPT_HANDLER( 0x77, 0 );
        SET_INTERRUPT_HANDLER( 0x78, 0 );
        SET_INTERRUPT_HANDLER( 0x79, 0 );
        SET_INTERRUPT_HANDLER( 0x7a, 0 );
        SET_INTERRUPT_HANDLER( 0x7b, 0 );
        SET_INTERRUPT_HANDLER( 0x7c, 0 );
        SET_INTERRUPT_HANDLER( 0x7d, 0 );
        SET_INTERRUPT_HANDLER( 0x7e, 0 );
        SET_INTERRUPT_HANDLER( 0x7f, 0 );
        SET_SYSTEM_INTERRUPT_HANDLER( 0x80, 0 );     // 系统调用
        SET_INTERRUPT_HANDLER( 0x81, 0 );
        SET_INTERRUPT_HANDLER( 0x82, 0 );
        SET_INTERRUPT_HANDLER( 0x83, 0 );
        SET_INTERRUPT_HANDLER( 0x84, 0 );
        SET_INTERRUPT_HANDLER( 0x85, 0 );
        SET_INTERRUPT_HANDLER( 0x86, 0 );
        SET_INTERRUPT_HANDLER( 0x87, 0 );
        SET_INTERRUPT_HANDLER( 0x88, 0 );
        SET_INTERRUPT_HANDLER( 0x89, 0 );
        SET_INTERRUPT_HANDLER( 0x8a, 0 );
        SET_INTERRUPT_HANDLER( 0x8b, 0 );
        SET_INTERRUPT_HANDLER( 0x8c, 0 );
        SET_INTERRUPT_HANDLER( 0x8d, 0 );
        SET_INTERRUPT_HANDLER( 0x8e, 0 );
        SET_INTERRUPT_HANDLER( 0x8f, 0 );
        SET_INTERRUPT_HANDLER( 0x90, 0 );
        SET_INTERRUPT_HANDLER( 0x91, 0 );
        SET_INTERRUPT_HANDLER( 0x92, 0 );
        SET_INTERRUPT_HANDLER( 0x93, 0 );
        SET_INTERRUPT_HANDLER( 0x94, 0 );
        SET_INTERRUPT_HANDLER( 0x95, 0 );
        SET_INTERRUPT_HANDLER( 0x96, 0 );
        SET_INTERRUPT_HANDLER( 0x97, 0 );
        SET_INTERRUPT_HANDLER( 0x98, 0 );
        SET_INTERRUPT_HANDLER( 0x99, 0 );
        SET_INTERRUPT_HANDLER( 0x9a, 0 );
        SET_INTERRUPT_HANDLER( 0x9b, 0 );
        SET_INTERRUPT_HANDLER( 0x9c, 0 );
        SET_INTERRUPT_HANDLER( 0x9d, 0 );
        SET_INTERRUPT_HANDLER( 0x9e, 0 );
        SET_INTERRUPT_HANDLER( 0x9f, 0 );
        SET_INTERRUPT_HANDLER( 0xa0, 0 );
        SET_INTERRUPT_HANDLER( 0xa1, 0 );
        SET_INTERRUPT_HANDLER( 0xa2, 0 );
        SET_INTERRUPT_HANDLER( 0xa3, 0 );
        SET_INTERRUPT_HANDLER( 0xa4, 0 );
        SET_INTERRUPT_HANDLER( 0xa5, 0 );
        SET_INTERRUPT_HANDLER( 0xa6, 0 );
        SET_INTERRUPT_HANDLER( 0xa7, 0 );
        SET_INTERRUPT_HANDLER( 0xa8, 0 );
        SET_INTERRUPT_HANDLER( 0xa9, 0 );
        SET_INTERRUPT_HANDLER( 0xaa, 0 );
        SET_INTERRUPT_HANDLER( 0xab, 0 );
        SET_INTERRUPT_HANDLER( 0xac, 0 );
        SET_INTERRUPT_HANDLER( 0xad, 0 );
        SET_INTERRUPT_HANDLER( 0xae, 0 );
        SET_INTERRUPT_HANDLER( 0xaf, 0 );
        SET_INTERRUPT_HANDLER( 0xb0, 0 );
        SET_INTERRUPT_HANDLER( 0xb1, 0 );
        SET_INTERRUPT_HANDLER( 0xb2, 0 );
        SET_INTERRUPT_HANDLER( 0xb3, 0 );
        SET_INTERRUPT_HANDLER( 0xb4, 0 );
        SET_INTERRUPT_HANDLER( 0xb5, 0 );
        SET_INTERRUPT_HANDLER( 0xb6, 0 );
        SET_INTERRUPT_HANDLER( 0xb7, 0 );
        SET_INTERRUPT_HANDLER( 0xb8, 0 );
        SET_INTERRUPT_HANDLER( 0xb9, 0 );
        SET_INTERRUPT_HANDLER( 0xba, 0 );
        SET_INTERRUPT_HANDLER( 0xbb, 0 );
        SET_INTERRUPT_HANDLER( 0xbc, 0 );
        SET_INTERRUPT_HANDLER( 0xbd, 0 );
        SET_INTERRUPT_HANDLER( 0xbe, 0 );
        SET_INTERRUPT_HANDLER( 0xbf, 0 );
        SET_INTERRUPT_HANDLER( 0xc0, 0 );
        SET_INTERRUPT_HANDLER( 0xc1, 0 );
        SET_INTERRUPT_HANDLER( 0xc2, 0 );
        SET_INTERRUPT_HANDLER( 0xc3, 0 );
        SET_INTERRUPT_HANDLER( 0xc4, 0 );
        SET_INTERRUPT_HANDLER( 0xc5, 0 );
        SET_INTERRUPT_HANDLER( 0xc6, 0 );
        SET_INTERRUPT_HANDLER( 0xc7, 0 );
        SET_INTERRUPT_HANDLER( 0xc8, 0 );
        SET_INTERRUPT_HANDLER( 0xc9, 0 );
        SET_INTERRUPT_HANDLER( 0xca, 0 );
        SET_INTERRUPT_HANDLER( 0xcb, 0 );
        SET_INTERRUPT_HANDLER( 0xcc, 0 );
        SET_INTERRUPT_HANDLER( 0xcd, 0 );
        SET_INTERRUPT_HANDLER( 0xce, 0 );
        SET_INTERRUPT_HANDLER( 0xcf, 0 );
        SET_INTERRUPT_HANDLER( 0xd0, 0 );
        SET_INTERRUPT_HANDLER( 0xd1, 0 );
        SET_INTERRUPT_HANDLER( 0xd2, 0 );
        SET_INTERRUPT_HANDLER( 0xd3, 0 );
        SET_INTERRUPT_HANDLER( 0xd4, 0 );
        SET_INTERRUPT_HANDLER( 0xd5, 0 );
        SET_INTERRUPT_HANDLER( 0xd6, 0 );
        SET_INTERRUPT_HANDLER( 0xd7, 0 );
        SET_INTERRUPT_HANDLER( 0xd8, 0 );
        SET_INTERRUPT_HANDLER( 0xd9, 0 );
        SET_INTERRUPT_HANDLER( 0xda, 0 );
        SET_INTERRUPT_HANDLER( 0xdb, 0 );
        SET_INTERRUPT_HANDLER( 0xdc, 0 );
        SET_INTERRUPT_HANDLER( 0xdd, 0 );
        SET_INTERRUPT_HANDLER( 0xde, 0 );
        SET_INTERRUPT_HANDLER( 0xdf, 0 );
        SET_INTERRUPT_HANDLER( 0xe0, 0 );
        SET_INTERRUPT_HANDLER( 0xe1, 0 );
        SET_INTERRUPT_HANDLER( 0xe2, 0 );
        SET_INTERRUPT_HANDLER( 0xe3, 0 );
        SET_INTERRUPT_HANDLER( 0xe4, 0 );
        SET_INTERRUPT_HANDLER( 0xe5, 0 );
        SET_INTERRUPT_HANDLER( 0xe6, 0 );
        SET_INTERRUPT_HANDLER( 0xe7, 0 );
        SET_INTERRUPT_HANDLER( 0xe8, 0 );
        SET_INTERRUPT_HANDLER( 0xe9, 0 );
        SET_INTERRUPT_HANDLER( 0xea, 0 );
        SET_INTERRUPT_HANDLER( 0xeb, 0 );
        SET_INTERRUPT_HANDLER( 0xec, 0 );
        SET_INTERRUPT_HANDLER( 0xed, 0 );
        SET_INTERRUPT_HANDLER( 0xee, 0 );
        SET_INTERRUPT_HANDLER( 0xef, 0 );
        SET_INTERRUPT_HANDLER( 0xf0, 0 );
        SET_INTERRUPT_HANDLER( 0xf1, 0 );
        SET_INTERRUPT_HANDLER( 0xf2, 0 );
        SET_INTERRUPT_HANDLER( 0xf3, 0 );
        SET_INTERRUPT_HANDLER( 0xf4, 0 );
        SET_INTERRUPT_HANDLER( 0xf5, 0 );
        SET_INTERRUPT_HANDLER( 0xf6, 0 );
        SET_INTERRUPT_HANDLER( 0xf7, 0 );
        SET_INTERRUPT_HANDLER( 0xf8, 0 );
        SET_INTERRUPT_HANDLER( 0xf9, 0 );
        SET_INTERRUPT_HANDLER( 0xfa, 0 );
        SET_INTERRUPT_HANDLER( 0xfb, 0 );
        SET_INTERRUPT_HANDLER( 0xfc, 0 );
        SET_INTERRUPT_HANDLER( 0xfd, 0 );
        SET_INTERRUPT_HANDLER( 0xfe, 0 );
        SET_INTERRUPT_HANDLER( 0xff, 0 );
        /* 挂载 idt*/
        sout[ ostream::HeadLevel::SYSTEM ] << "Loading the interrupt descriptor table." << endl;
        this->load( );

        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the interrupt descriptor table management." << Lib::IO::endl;
    }

    auto InterruptDescriptorManagement::load( VOID ) CONST->VOID {
        ASM( "lidt %0" ::"m"( this->xdtr ) );
        return;
    }
    auto InterruptDescriptorManagement::read( VOID ) CONST->Lib::Types::Ptr< InterruptDescriptor > {
        // 获取idt地址并返回
        ASM( "sidt %0" ::"m"( this->xdtr ) );
        return this->xdtr.descriptor;
    }
    auto InterruptDescriptorManagement::make_descriptor( IN CONST Lib::Types::uint16_t index,
                                                         IN CONST Lib::Types::uint64_t entry_point,
                                                         IN CONST Lib::Types::uint16_t selector,
                                                         IN CONST Lib::Types::uint8_t ist,
                                                         IN CONST Lib::Types::uint8_t attributes )
        ->Lib::Types::L_Ref< InterruptDescriptor > {
        Lib::Types::Ptr< Architecture::CPU::InterruptDescriptor > entry = &this->xdtr.descriptor[ index ];
        entry->offset_low = ( entry_point & 0xffff );
        entry->selector = selector;
        entry->ist = ist;
        entry->attribute = attributes;
        entry->offset_middle = static_cast< decltype( entry->offset_middle ) >( ( entry_point >> 16 ) & 0xffff );
        entry->offset_high = static_cast< decltype( entry->offset_high ) >( ( entry_point >> 32 ) & 0xffffffff );
        return *entry;
    }
    auto InterruptDescriptorManagement::display_registers( IN Lib::Types::Ptr< CONST Platform::RegisterFrame > registers )->VOID {
        using namespace ::QuantumNEC::Lib::IO;
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RAX -> %#018lx\tRBX -> %#018lx\tRCX -> %#018lx\tRDX -> %#018lx\n\r", registers->rax, registers->rbx, registers->rcx, registers->rdx );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "R8  -> %#018lx\tR9  -> %#018lx\tR10 -> %#018lx\tR11 -> %#018lx\tR12 -> %#018lx\tR13 -> %#018lx\tR14 -> %#018lx\tR15 -> %#018lx\n\r", registers->r8, registers->r9, registers->r10, registers->r11, registers->r12, registers->r13, registers->r14, registers->r15 );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RDI -> %#018lx\tRSI -> %#018lx\tRBP -> %#018lx\n\r", registers->rdi, registers->rsi, registers->rbp );
        Architecture::Platform::ControlRegisterFrame control_registers_frame { };
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "CR0 -> %#018lx\tCR2 -> %#018lx\tCR3 -> %#018lx\tCR4 -> %#018lx\n\r", control_registers_frame.cr0, control_registers_frame.cr2, control_registers_frame.cr3, control_registers_frame.cr4 );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "DS -> %#018lx\tES -> %#018lx\tFS -> %#018lx\tGS -> %#018lx\n\r", registers->ds, registers->es, registers->fs, registers->gs );
        return;
    }
    auto InterruptDescriptorManagement::set_interrupt_handler( IN CONST Lib::Types::uint8_t irq, IN InterruptEntry handle )->VOID {
        if ( irq > 0x1d ) {
            interrupt_entry_table[ irq ] = handle;
        }
        return;
    }
    auto InterruptDescriptorManagement::set_exception_handler( IN CONST Lib::Types::uint8_t irq, IN InterruptEntry handle )->VOID {
        if ( irq < 0x1e ) {
            interrupt_entry_table[ irq ] = handle;
        }
        return;
    }
    GlobalSegmentDescriptorManagement::GlobalSegmentDescriptorManagement( IN GlobalSegmentDescriptor _descriptor[ Platform::GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT ][ Platform::SEGMENT_DESCRIPTOR_COUNT ], IN Lib::Types::uint16_t _size ) noexcept :
        DescriptorManagement< GlobalSegmentDescriptor > {
            _descriptor[ 0 ], _size
        } {
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the global segment descriptor table management." << endl;

        for ( Lib::Types::size_t i { }, tss_base_low { }, tss_base_high { }; i < Platform::GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT; ++i ) {
            tss_base_low = ( reinterpret_cast< Lib::Types::uint64_t >( &this->tss[ i ] ) ) & 0xffffffff;
            tss_base_high = ( reinterpret_cast< Lib::Types::uint64_t >( &this->tss[ i ] ) >> 32 ) & 0xffffffff;
            this->tss[ i ].io_map_base_address = ( sizeof( TaskStateSegmentDescriptor ) << 16 );
            // 设置GDT里的LDT
            this->make_descriptor( _descriptor[ i ], 0, 0, 0, 0 );                                                                                  /*0	        NULL descriptor                                                             0x00*/
            this->make_descriptor( _descriptor[ i ], 1, 0, 0, Platform::AR_CODE64 );                                                                /*1	        KERNEL	Code	                                            64-bit	Segment 0x08*/
            this->make_descriptor( _descriptor[ i ], 2, 0, 0, Platform::AR_DATA64 );                                                                /*2	        KERNEL	Data	                                            64-bit	Segment 0x10*/
            this->make_descriptor( _descriptor[ i ], 3, 0, 0xfffff, Platform::AR_CODE32_DPL3 );                                                     /*3	        USER	Code	                                            32-bit	Segment 0x18*/
            this->make_descriptor( _descriptor[ i ], 4, 0, 0xfffff, Platform::AR_DATA32_DPL3 );                                                     /*4	        USER	Data	                                            32-bit	Segment 0x20*/
            this->make_descriptor( _descriptor[ i ], 5, 0, 0, Platform::AR_CODE64_DPL3 );                                                           /*5	        USER	Code	                                            64-bit	Segment 0x28*/
            this->make_descriptor( _descriptor[ i ], 6, 0, 0, Platform::AR_DATA64_DPL3 );                                                           /*6	        USER	Data	                                            64-bit	Segment 0x30*/
            this->make_descriptor( _descriptor[ i ], 7, 0, 0xfffff, Platform::AR_CODE32 );                                                          /*7	        KERNEL	Code	                                            32-bit	Segment	0x38*/
            this->make_descriptor( _descriptor[ i ], 8, 0, 0xfffff, Platform::AR_DATA32 );                                                          /*8	        KERNEL	Data	                                            32-bit	Segment	0x40*/
            this->make_descriptor( _descriptor[ i ], 9, 0, 0, 0 );                                                                                  /*9	        NULL descriptor                                                             0x48*/
            this->make_descriptor( _descriptor[ i ], 10, tss_base_low & 0xffffffff, sizeof( TaskStateSegmentDescriptor ) - 1, Platform::AR_TSS64 ); /*10 ~ 11   TSS (10->low 11->high, jmp one segment <9>)                 64-bit          0x50*/
            Lib::STL::memcpy( &_descriptor[ i ][ 11 ], &tss_base_high, 8 );
        }
        sout[ ostream::HeadLevel::SYSTEM ] << "Loading the global segment descriptor table." << endl;
        // 加载GDT
        this->load( );
        // 加载全局段中的TSS
        sout[ ostream::HeadLevel::SYSTEM ] << "Loading the task state segment in global segment." << endl;
        this->load_tr( Platform::SELECTOR_TSS );
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the global segment descriptor table management." << Lib::IO::endl;
    }
    auto GlobalSegmentDescriptorManagement::load( VOID ) CONST->VOID {
        ASM( "lgdt %[GDTR]" ::[ GDTR ] "m"( this->xdtr ) );
        ASM(
            "MOVQ %%RAX, %%DS \n\t"
            "MOVQ %%RAX, %%ES \n\t"
            "MOVQ %%RAX, %%FS \n\t"
            "MOVQ %%RAX, %%GS \n\t"
            "MOVQ %%RAX, %%SS \n\t"
            "PUSHQ %[SELECTOR_CODE64] \n\t"
            "LEAQ .next(%%RIP),%%RAX \n\t"
            "PUSHQ %%RAX \n\t"
            "LRETQ \n\r"
            ".next: \n\t"
            :
            : [SELECTOR_CODE64] "i"( Platform::SELECTOR_CODE64_KERNEL ),
              [SELECTOR_DATA64] "rax"( Platform::SELECTOR_DATA64_KERNEL )
            : );
        return;
    }
    auto GlobalSegmentDescriptorManagement::read( VOID ) CONST->Lib::Types::Ptr< GlobalSegmentDescriptor > {
        ASM( "sgdt %0" ::"m"( this->xdtr ) );
        return this->xdtr.descriptor;
    }
    auto GlobalSegmentDescriptorManagement::load_tr( IN Lib::Types::size_t segment )->VOID {
        ASM( "ltr %%ax" ::"a"( segment ) : "memory" );
    }
    auto GlobalSegmentDescriptorManagement::make_descriptor( IN Lib::Types::Ptr< GlobalSegmentDescriptor > descriptor, IN Lib::Types::uint8_t index, IN Lib::Types::uint64_t base, IN Lib::Types::uint64_t limit, IN Lib::Types::uint64_t access )->Lib::Types::L_Ref< GlobalSegmentDescriptor > {
        Lib::Types::Ptr< GlobalSegmentDescriptor > entry { &descriptor[ index ] };
        // 分别设置描述符
        entry->limit_low = limit & 0xffff;
        entry->base_low = base & 0xffff;
        entry->base_middle = ( base & 0xff0000 ) >> 16;
        entry->access_right = access & 0xff;
        entry->limit_high = ( ( limit >> 16 ) & 0xf ) | ( ( access >> 8 ) & 0xf0 );
        entry->base_high = ( base >> 24 ) & 0xff;
        return *entry;
    }
}
