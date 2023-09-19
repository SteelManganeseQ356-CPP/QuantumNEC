#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Lib/STL/cstring>
#include <Utils/asm.hpp>
/**
 * @brief 通用的中断处理函数,一般用在异常出现时的处理
 * @param frame 保存的栈寄存器数据
 */

#define HANDLE_ENTRY( n )                              \
    _C_LINK auto InterruptHandler##n( VOID ) -> VOID { \
        ASM( "JMP interruptHandler" #n "\n\r" );       \
    }
HANDLE_ENTRY( 0x00 );     //	#DE	除以0
HANDLE_ENTRY( 0x01 );     //	#DB	调试
HANDLE_ENTRY( 0x02 );     //	--- NMI中断
HANDLE_ENTRY( 0x03 );     //	#BP	断点
HANDLE_ENTRY( 0x04 );     //	#OF 溢出
HANDLE_ENTRY( 0x05 );     //	#BR 数组索引超限
HANDLE_ENTRY( 0x06 );     //	#UD	未定义指令
HANDLE_ENTRY( 0x07 );     //	#NM	设备未就绪
HANDLE_ENTRY( 0x08 );     //    #DF	双重错误
HANDLE_ENTRY( 0x09 );     //    --- 协处理器错误
HANDLE_ENTRY( 0x0a );     //    #TS	无效的TSS
HANDLE_ENTRY( 0x0b );     //    #NP	段不存在
HANDLE_ENTRY( 0x0c );     //    #SS 堆栈段故障
HANDLE_ENTRY( 0x0d );     //    #GP 一般保护故障
HANDLE_ENTRY( 0x0e );     //    #PF	缺页
HANDLE_ENTRY( 0x0f );     //    --- intel保留，请勿使用
HANDLE_ENTRY( 0x10 );     //    #MF	x87浮点数异常
HANDLE_ENTRY( 0x11 );     //	#AC 内存数据引用未对齐 仅在Ring3触发
HANDLE_ENTRY( 0x12 );     //	#MC	处理器内部错误
HANDLE_ENTRY( 0x13 );     //    #XM	SIMD浮点数异常
HANDLE_ENTRY( 0x14 );     //    #CP 处理器内部错误
HANDLE_ENTRY( 0x15 );     //    #HV 虚拟机注入的异常
HANDLE_ENTRY( 0x16 );     //    #VC VMM 通信失败
HANDLE_ENTRY( 0x17 );     //    #SX 安全异常
HANDLE_ENTRY( 0x18 );
HANDLE_ENTRY( 0x19 );
HANDLE_ENTRY( 0x1a );
HANDLE_ENTRY( 0x1b );
HANDLE_ENTRY( 0x1c );
HANDLE_ENTRY( 0x1d );
HANDLE_ENTRY( 0x1e );
HANDLE_ENTRY( 0x1f );
HANDLE_ENTRY( 0x20 );     // 时钟中断对应的入口
HANDLE_ENTRY( 0x21 );     // 键盘中断对应的入口
HANDLE_ENTRY( 0x22 );     // 级联用的
HANDLE_ENTRY( 0x23 );     // 串口2对应的入口
HANDLE_ENTRY( 0x24 );     // 串口1对应的入口
HANDLE_ENTRY( 0x25 );     // 并口2对应的入口
HANDLE_ENTRY( 0x26 );     // 软盘对应的入口
HANDLE_ENTRY( 0x27 );     // 并口1对应的入口
HANDLE_ENTRY( 0x28 );     // 实时时钟对应的入口
HANDLE_ENTRY( 0x29 );     // 重定向
HANDLE_ENTRY( 0x2a );     // 保留
HANDLE_ENTRY( 0x2b );     // 保留
HANDLE_ENTRY( 0x2c );     // ps/2鼠标
HANDLE_ENTRY( 0x2d );     // x87浮点单元异常
HANDLE_ENTRY( 0x2e );     // 硬盘
HANDLE_ENTRY( 0x2f );     // 保留
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
HANDLE_ENTRY( 0x40 );     // 系统调用
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
HANDLE_ENTRY( 0x80 );
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

/**
 * @brief 定义异常接收函数
 */

/**
 * @brief 设置中断入口（Trap）
 */
#define SET_TRAP_HANDLER( index, ist )                       \
    this->makeDescriptor(                                    \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        ( 1 << 3 ),                                          \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::TRAP ) )
/**
 * @brief 设置中断入口（System）
 */
#define SET_SYSTEM_HANDLER( index, ist )                     \
    this->makeDescriptor(                                    \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        ( 1 << 3 ),                                          \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::SYSTEM ) )
/**
 * @brief 设置中断入口（System——Interrupt）
 */
#define SET_SYSTEM_INTERRUPT_HANDLER( index, ist )           \
    this->makeDescriptor(                                    \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        ( 1 << 3 ),                                          \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::SYSTEM_INTERRUPT ) )
/**
 * @brief 设置中断入口（Interrupt）
 */
#define SET_INTERRUPT_HANDLER( index, ist )                  \
    this->makeDescriptor(                                    \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        ( 1 << 3 ),                                          \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::INTERRUPT ) )

PUBLIC namespace QuantumNEC::Architecture::CPU {
    PUBLIC using namespace Lib::IO;
    PRIVATE Lib::Types::Ptr< CONST Lib::Types::char_t > interruptName[ Platform::IDT_MAX ];     // 各个异常的名字
    _C_LINK auto generalInterruptHandler( ::QuantumNEC::Lib::Types::Ptr< CONST ::QuantumNEC::Architecture::CPU::InterruptFrame > frame )->VOID {
        using namespace QuantumNEC::Lib;
        using namespace QuantumNEC::Lib::IO;

#ifndef APIC
        if ( frame->irq == 0x27 || frame->irq == 0x2f )     // 0x2f是从片8259A上的最后一个irq引脚，保留
            return;                                         // IRQ7和IRQ15会产生伪中断(spurious interrupt),无须处理。
#endif
        if ( ::QuantumNEC::Architecture::CPU::interruptEntryTable[ frame->irq ] == reinterpret_cast< Types::Ptr< VOID > >( generalInterruptHandler ) ) {
            sout[ ostream::HeadLevel::ERROR ] << "IRQ_" << frame->irq << ":" << QuantumNEC::Architecture::CPU::interruptName[ frame->irq ] << endl;

            // 显示各个寄存器值
            sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "Rflags -> %#018lx\n\r", frame->rflags );
            sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RIP -> %#018lx\tRSP -> %#018lx\tSS  -> %#018lx\tCS  -> %#018lx\n\r", frame->rip, frame->rsp, frame->ss, frame->cs );
            ::QuantumNEC::Architecture::CPU::InterruptDescriptorManagement::displayRegs( &frame->regs );
            if ( frame->errCode ) {
                sout[ ostream::HeadLevel::INFO ] << "Error Code -> " << frame->errCode << endl;
            }
            if ( frame->irq == 0x0a || frame->irq == 0x0b || frame->irq == 0x0c ) /* #TS的特殊处理 */
            {
                if ( frame->errCode & 0x01 ) {
                    sout[ ostream::HeadLevel::INFO ] << "The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception." << endl;
                }
                if ( frame->errCode & 0x02 ) {
                    sout[ ostream::HeadLevel::INFO ] << "Refers to a gate descriptor in the IDT." << endl;
                }
                else {
                    sout[ ostream::HeadLevel::INFO ] << "Refers to a descriptor in the GDT or the current LDT." << endl;
                }
                if ( !( frame->errCode & 0x02 ) ) {
                    if ( frame->errCode & 0x04 ) {
                        sout[ ostream::HeadLevel::INFO ] << "Refers to a segment or gate descriptor in the LDT." << endl;
                    }
                    else {
                        sout[ ostream::HeadLevel::INFO ] << "Refers to a descriptor in the current GDT." << endl;
                    }
                }
                sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "Segment Selector Index:%#010x\n", frame->errCode & 0xfff8 );
            }

            if ( frame->irq == 0x0e ) /* #PF的特殊处理 */
            {
                unsigned long int cr2 { };
                ASM( "MOVQ %%cr2,  %0\n" : "=r"( cr2 ) : : "memory" );
                sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "CR2 -> %#018lx\n", cr2 );
                sout[ ostream::HeadLevel::INFO ];
                if ( !( frame->errCode & 0x01 ) ) {
                    sout << "Page Not-Present, ";
                }
                if ( frame->errCode & 0x02 ) {
                    sout << "Write Cause Fault, ";
                }
                else {
                    sout << "Read Cause Fault, ";
                }
                if ( frame->errCode & 0x04 ) {
                    sout << "Fault in user(3), ";
                }
                else {
                    sout << "Fault in supervisor(0,1,2), ";
                }
                if ( frame->errCode & 0x08 ) {
                    sout << "Reserved Bit Cause Fault";
                }
                if ( frame->errCode & 0x10 ) {
                    sout << "Instruction fetch Cause Fault";
                }
                endl( sout );
            }
            while ( TRUE )     // 因为进入异常处理时已经关闭了中断所以直接阻塞即可
                ::QuantumNEC::Utils::hlt( );
        }
        else {
            interruptEntryTable[ frame->irq ]( frame );
            return;
        }
    }
    InterruptDescriptorManagement::InterruptDescriptorManagement( IN Lib::Types::Ptr< InterruptDescriptor > _descriptor, IN Lib::Types::uint16_t _num ) :
        Platform::DescriptorManagement< InterruptDescriptor > {
        _descriptor, _num
    }
    {
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the Interrupt Descriptor Table Management." << Lib::IO::endl;
        Utils::cli( );     // 关中断
        Lib::STL::memset( this->xdtr.descriptor, 0, Platform::IDT_MAX * sizeof( InterruptDescriptor ) );
        for ( Lib::Types::uint16_t i { }; i < Platform::IDT_MAX; ++i ) {
            interruptName[ i ] = "unknown";                         // 先统一赋值为unknown
            interruptEntryTable[ i ] = generalInterruptHandler;     // 默认为generalIntrHandler
        }
        interruptName[ 0 ] = "#DE Divide Error";
        interruptName[ 1 ] = "#DB Debug Exception";
        interruptName[ 2 ] = "--- NMI Interrupt";
        interruptName[ 3 ] = "#BP Breakpoint Exception";
        interruptName[ 4 ] = "#OF Overflow Exception";
        interruptName[ 5 ] = "#BR BOUND Range Exceeded Exception";
        interruptName[ 6 ] = "#UD Invalid Opcode Exception";
        interruptName[ 7 ] = "#NM Device Not Available Exception";
        interruptName[ 8 ] = "#DF Double Fault Exception";
        interruptName[ 9 ] = "--- Coprocessor Segment Overrun";
        interruptName[ 10 ] = "#TS Invalid TSS Exception";
        interruptName[ 11 ] = "#NP Segment Not Present";
        interruptName[ 12 ] = "#SS Stack Fault Exception";
        interruptName[ 13 ] = "#GP General Protection Exception";
        interruptName[ 14 ] = "#PF Page-Fault Exception";
        interruptName[ 15 ] = "--- Intel Reserved";     // 第15项是intel保留项，未使用
        interruptName[ 16 ] = "#MF x87 FPU Floating-Point Error";
        interruptName[ 17 ] = "#AC Alignment Check Exception";
        interruptName[ 18 ] = "#MC Machine-Check Exception";
        interruptName[ 19 ] = "#XF SIMD Floating-Point Exception";
        interruptName[ 20 ] = "#CP Control protection exceptions";
        interruptName[ 21 ] = "#HV Exception injected by the virtual machine";
        interruptName[ 22 ] = "#VC VMM communication failed";
        interruptName[ 23 ] = "#SX Security exceptions";

        /* 注册所有入口函数到中断描述符表 */

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
        SET_INTERRUPT_HANDLER( 0x20, 0 );     // 时钟中断对应的入口
        SET_INTERRUPT_HANDLER( 0x21, 0 );     // 键盘中断对应的入口
        SET_INTERRUPT_HANDLER( 0x22, 0 );     // 级联用的
        SET_INTERRUPT_HANDLER( 0x23, 0 );     // 串口2对应的入口
        SET_INTERRUPT_HANDLER( 0x24, 0 );     // 串口1对应的入口
        SET_INTERRUPT_HANDLER( 0x25, 0 );     // 并口2对应的入口
        SET_INTERRUPT_HANDLER( 0x26, 0 );     // 软盘对应的入口
        SET_INTERRUPT_HANDLER( 0x27, 0 );     // 并口1对应的入口
        SET_INTERRUPT_HANDLER( 0x28, 0 );     // 实时时钟对应的入口
        SET_INTERRUPT_HANDLER( 0x29, 0 );     // 重定向
        SET_INTERRUPT_HANDLER( 0x2a, 0 );     // 保留
        SET_INTERRUPT_HANDLER( 0x2b, 0 );     // 保留
        SET_INTERRUPT_HANDLER( 0x2c, 0 );     // ps/2鼠标
        SET_INTERRUPT_HANDLER( 0x2d, 0 );     // x87浮点单元异常
        SET_INTERRUPT_HANDLER( 0x2e, 0 );     // 硬盘
        SET_INTERRUPT_HANDLER( 0x2f, 0 );     // 保留
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
        SET_SYSTEM_INTERRUPT_HANDLER( 0x40, 0 );     // 系统调用
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
        SET_INTERRUPT_HANDLER( 0x80, 0 );
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
        sout[ ostream::HeadLevel::SYSTEM ] << "Loading the Interrupt Descriptor Table." << endl;
        this->load( );

        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the  Interrupt Descriptor Table Management." << Lib::IO::endl;
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
    auto InterruptDescriptorManagement::makeDescriptor( IN CONST Lib::Types::uint16_t index,
                                                        IN CONST Lib::Types::uint64_t entryPoint,
                                                        IN CONST Lib::Types::uint16_t selector,
                                                        IN CONST Lib::Types::uint8_t ist,
                                                        IN CONST Lib::Types::uint8_t attributes )
        ->Lib::Types::L_Ref< InterruptDescriptor > {
        Lib::Types::Ptr< Architecture::CPU::InterruptDescriptor > entry = &this->xdtr.descriptor[ index ];
        entry->LowOffset = ( entryPoint & 0xffff );
        entry->selector = selector;
        entry->ist = ist;
        entry->attribute = attributes;
        entry->middleOffset = ( ( entryPoint >> 16 ) & 0xffff );
        entry->highOffset = ( ( entryPoint >> 32 ) & 0xffffffff );
        return *entry;
    }
    auto InterruptDescriptorManagement::displayRegs( IN Lib::Types::Ptr< CONST Platform::RegisterFrame > regs )->VOID {
        using namespace ::QuantumNEC::Lib::IO;
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RAX -> %#018lx\tRBX -> %#018lx\tRCX -> %#018lx\tRDX -> %#018lx\n\r", regs->rax, regs->rbx, regs->rcx, regs->rdx );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "R8  -> %#018lx\tR9  -> %#018lx\tR10 -> %#018lx\tR11 -> %#018lx\tR12 -> %#018lx\tR13 -> %#018lx\tR14 -> %#018lx\tR15 -> %#018lx\n\r", regs->r8, regs->r9, regs->r10, regs->r11, regs->r12, regs->r13, regs->r14, regs->r15 );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RDI -> %#018lx\tRSI -> %#018lx\tRBP -> %#018lx\n\r", regs->rdi, regs->rsi, regs->rbp );
        return;
    }
    auto InterruptDescriptorManagement::setHandle( IN CONST Lib::Types::uint8_t nr, IN Lib::Types::FuncPtr< VOID, Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > > handle )->VOID {
        if ( nr > 0x17 ) {
            interruptEntryTable[ nr ] = handle;
        }
        return;
    }
    GlobalDescriptorManagement::GlobalDescriptorManagement( IN Lib::Types::Ptr< SegmentDescriptor > _descriptor, IN Lib::Types::uint16_t _size ) :
        DescriptorManagement< SegmentDescriptor > {
        _descriptor, _size
    }
    {
                Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the Global Descriptor Table Management." << Lib::IO::endl;
        // 清零
        Lib::STL::memset( &this->xdtr.descriptor, 0, sizeof( *this->xdtr.descriptor ) );

        // 设置GDT
        for ( Lib::Types::uint32_t i = { }; i < 4; ++i ) {
            makeDescriptor( this->xdtr.descriptor, 1 + i * 2, 0u, i, SEGFLAG_EXCUTEABLE );
            makeDescriptor( this->xdtr.descriptor, 1 + i * 2 + 1, 0u, i, SEGFLAG_RW );
        }
        makeDescriptor( this->xdtr.descriptor, 0, &TaskStateSegmentDescriptorManagement::tss, sizeof( TaskStateSegmentDescriptor64 ), static_cast< Lib::Types::uint8_t >( TaskStateSegmentDescriptorManagement::TaskStateSegmentDescriptorAttribute::AVAILABLE ) );
        makeDescriptor( this->xdtr.descriptor, 1, 0u, 0, Platform::AR_CODE64 );
        makeDescriptor( this->xdtr.descriptor, 2, 0u, 0, Platform::AR_DATA64 );
        makeDescriptor( this->xdtr.descriptor, 3, 0u, 0xfffff, Platform::AR_CODE32_DPL3 );
        makeDescriptor( this->xdtr.descriptor, 4, 0u, 0xfffff, Platform::AR_DATA32_DPL3 );
        makeDescriptor( this->xdtr.descriptor, 5, 0u, 0, Platform::AR_CODE64_DPL3 );
        makeDescriptor( this->xdtr.descriptor, 6, 0u, 0, Platform::AR_DATA64_DPL3 );
        makeDescriptor( this->xdtr.descriptor, 7, 0u, 0xfffff, Platform::AR_CODE32 );
        makeDescriptor( this->xdtr.descriptor, 8, 0u, 0xfffff, Platform::AR_DATA32 );
        // 设置TSS
        sout[ ostream::HeadLevel::START ] << "Initializethe the Task State Segment." << endl;
        TaskStateSegmentDescriptorManagement tss_management { this->xdtr.descriptor };
        sout[ ostream::HeadLevel::SYSTEM ] << "Loading the Global Segment Descriptor Table." << endl;
        this->load( );
        sout[ ostream::HeadLevel::SYSTEM ] << "Setting the Task State Segment Descriptor." << endl;
        tss_management.set( );
        sout[ ostream::HeadLevel::OK ] << "Initialize the Task State Segment." << endl;
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the Global Descriptor Table Management." << Lib::IO::endl;
    }
    auto GlobalDescriptorManagement::load( VOID ) CONST->VOID {
        ASM( "lgdt %0" ::"m"( this->xdtr ) );
        return;
    }
    auto GlobalDescriptorManagement::read( VOID ) CONST->Lib::Types::Ptr< SegmentDescriptor > {
        ASM( "sgdt %0" ::"m"( this->xdtr ) );
        return this->xdtr.descriptor;
    }
    auto GlobalDescriptorManagement::makeDescriptor( IN Lib::Types::Ptr< SegmentDescriptor > descriptor, IN Lib::Types::uint8_t index, IN Lib::Types::uint32_t base, IN Lib::Types::uint32_t limit, IN Lib::Types::uint32_t access )->Lib::Types::L_Ref< SegmentDescriptor > {
        Lib::Types::Ptr< SegmentDescriptor > entry { &descriptor[ index ] };
        entry->limit_low = ( limit & 0x0000ffff );
        entry->base_low = ( base & 0x0000ffff );
        entry->base_mid = ( ( base & 0x00ff0000 ) >> 16 );
        entry->access_right = ( access & 0x00ff );                                             // TYPE,S,DPL,P
        entry->limit_high = ( ( ( limit >> 16 ) & 0x0f ) | ( ( access >> 8 ) & 0x00f0 ) );     // AVL,L,D/B,G
        entry->base_high = ( ( base >> 24 ) & 0x00ff );
        return *entry;
    }
    auto GlobalDescriptorManagement::makeDescriptor( IN Lib::Types::Ptr< SegmentDescriptor > descriptor, IN Lib::Types::uint8_t index, IN Lib::Types::Ptr< VOID > base, IN Lib::Types::uint32_t limit, IN Lib::Types::uint8_t flag )->Lib::Types::L_Ref< TaskStateDescriptor > {
        Lib::Types::Ptr< TaskStateDescriptor > entry { reinterpret_cast< Lib::Types::Ptr< TaskStateDescriptor > >( &descriptor[ 9 + index * 2 ] ) };
        entry->isVaild = TRUE;
        entry->descType = static_cast< Lib::Types::uint32_t >( GlobalDescriptorManagement::GlobalDescriptorAttribute::SYSTEM );
        entry->privilege = 0;
        entry->flag = flag;
        entry->limit0 = ( limit & 0xffff );
        entry->limit1 = ( limit >> 16 ) & 0b1111;
        entry->base0 = ( (Lib::Types::uint64_t)base & 0xffff );
        entry->base1 = ( (Lib::Types::uint64_t)base >> 16 ) & 0xff;
        entry->base2 = ( (Lib::Types::uint64_t)base >> 24 ) & 0xff;
        entry->base3 = ( (Lib::Types::uint64_t)base >> 32 );
        return *entry;
    }
}
