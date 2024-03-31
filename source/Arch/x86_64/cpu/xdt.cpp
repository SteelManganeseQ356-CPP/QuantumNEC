#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/IO/Stream/iostream>
#include <Lib/STL/cstring>
#include <Utils/asm.hpp>

/**
 * @brief 通用的中断处理函数,一般用在异常出现时的处理
 * @param frame 保存的栈寄存器数据
 */
#define ZERO 0
#define ERROR_CODE 1
#define HANDLE_ENTRY( irq, error_code )                  \
    _C_LINK auto InterruptHandler##irq( VOID ) -> VOID { \
        ASM( "CLI\n\t" );                                \
        if ( error_code ) { ASM( "NOP\n\t" ); }          \
        else { ASM( "PUSHQ $0\n\t" ); }                  \
        ASM(                                             \
            "PUSHQ $" #irq "\n\t"                        \
            "PUSHQ %R15\n\t"                             \
            "PUSHQ %R14\n\t"                             \
            "PUSHQ %R13\n\t"                             \
            "PUSHQ %R12\n\t"                             \
            "PUSHQ %R11\n\t"                             \
            "PUSHQ %R10\n\t"                             \
            "PUSHQ %R9\n\t"                              \
            "PUSHQ %R8\n\t"                              \
            "PUSHQ %RDI\n\t"                             \
            "PUSHQ %RSI\n\t"                             \
            "PUSHQ %RBP\n\t"                             \
            "PUSHQ %RDX\n\t"                             \
            "PUSHQ %RCX\n\t"                             \
            "PUSHQ %RBX\n\t"                             \
            "PUSHQ %RAX \n\t"                            \
            "MOVQ  $0, %RAX\n\t"                         \
            "MOVQ  %GS, %RAX\n\t"                        \
            "PUSHQ %RAX\n\t"                             \
            "MOVQ  %FS, %RAX\n\t"                        \
            "PUSHQ %RAX\n\t"                             \
            "MOVQ  %ES, %RAX\n\t"                        \
            "PUSHQ %RAX\n\t"                             \
            "MOVQ  %DS, %RAX\n\t"                        \
            "PUSHQ %RAX\n\t"                             \
            "MOVQ  %CR0, %RAX\n\t"                       \
            "PUSHQ %RAX\n\t"                             \
            "MOVQ  %CR2, %RAX\n\t"                       \
            "PUSHQ %RAX\n\t"                             \
            "MOVQ  %CR3, %RAX\n\t"                       \
            "PUSHQ %RAX\n\t"                             \
            "MOVQ  %CR4, %RAX\n\t"                       \
            "PUSHQ %RAX\n\t"                             \
            "MOVQ %RSP, %RDI\n\t"                        \
            "CALLQ general_interrupt_handler\n\t"        \
            "JMP interrupt_exit\n\t" );                  \
    }
__asm__(     // 中断-处理返回断点
    ".GLOBAL process_switch\n\r"
    ".GLOBAL interrupt_exit\n\r"
    "interrupt_exit:\n\t"
    "POPQ %RAX\n\t"
    "POPQ %RAX\n\t"
    "POPQ %RAX\n\t"
    "POPQ %RAX\n\r"
    "process_switch:\n\t"
    "POPQ %RAX\n\t"
    "MOVQ %RAX, %DS\n\t"
    "POPQ %RAX\n\t"
    "MOVQ %RAX, %ES\n\t"
    "POPQ %RAX\n\t"
    "MOVQ %RAX, %FS\n\t"
    "POPQ %RAX\n\t"
    "MOVQ %RAX, %GS\n\t"
    "POPQ %RAX\n\t"
    "POPQ %RBX\n\t"
    "POPQ %RCX\n\t"
    "POPQ %RDX\n\t"
    "POPQ %RBP\n\t"
    "POPQ %RSI\n\t"
    "POPQ %RDI\n\t"
    "POPQ %R8\n\t"
    "POPQ %R9\n\t"
    "POPQ %R10\n\t"
    "POPQ %R11\n\t"
    "POPQ %R12\n\t"
    "POPQ %R13\n\t"
    "POPQ %R14\n\t"
    "POPQ %R15\n\t"
    "ADDQ $16, %RSP\n\t"
    "IRETQ\n\t" );
HANDLE_ENTRY( 0x00, ZERO );           //	#DE	除以0
HANDLE_ENTRY( 0x01, ZERO );           //	#DB	调试
HANDLE_ENTRY( 0x02, ZERO );           //	--- NMI中断
HANDLE_ENTRY( 0x03, ZERO );           //	#BP	断点
HANDLE_ENTRY( 0x04, ZERO );           //	#OF 溢出
HANDLE_ENTRY( 0x05, ZERO );           //	#BR 数组索引超限
HANDLE_ENTRY( 0x06, ZERO );           //	#UD	未定义指令
HANDLE_ENTRY( 0x07, ZERO );           //	#NM	设备未就绪
HANDLE_ENTRY( 0x08, ERROR_CODE );     //    #DF	双重错误
HANDLE_ENTRY( 0x09, ZERO );           //    --- 协处理器错误
HANDLE_ENTRY( 0x0a, ERROR_CODE );     //    #TS	无效的TSS
HANDLE_ENTRY( 0x0b, ERROR_CODE );     //    #NP	段不存在
HANDLE_ENTRY( 0x0c, ERROR_CODE );     //    #SS 堆栈段故障
HANDLE_ENTRY( 0x0d, ERROR_CODE );     //    #GP 一般保护故障
HANDLE_ENTRY( 0x0e, ERROR_CODE );     //    #PF	缺页
HANDLE_ENTRY( 0x0f, ZERO );           //    --- intel保留，请勿使用
HANDLE_ENTRY( 0x10, ZERO );           //    #MF	x87浮点数异常
HANDLE_ENTRY( 0x11, ERROR_CODE );     //	#AC 内存数据引用未对齐 仅在Ring3触发
HANDLE_ENTRY( 0x12, ZERO );           //	#MC	处理器内部错误
HANDLE_ENTRY( 0x13, ZERO );           //    #XM	SIMD浮点数异常
HANDLE_ENTRY( 0x14, ZERO );           //    #CP 处理器内部错误
HANDLE_ENTRY( 0x15, ERROR_CODE );     //    #HV 虚拟机注入的异常
HANDLE_ENTRY( 0x16, ZERO );           //    #VC VMM 通信失败
HANDLE_ENTRY( 0x17, ZERO );           //    #SX 安全异常
HANDLE_ENTRY( 0x18, ZERO );
HANDLE_ENTRY( 0x19, ZERO );
HANDLE_ENTRY( 0x1a, ZERO );
HANDLE_ENTRY( 0x1b, ZERO );
HANDLE_ENTRY( 0x1c, ZERO );
HANDLE_ENTRY( 0x1d, ZERO );
HANDLE_ENTRY( 0x1e, ZERO );
HANDLE_ENTRY( 0x1f, ZERO );
HANDLE_ENTRY( 0x20, ZERO );     // 时钟中断对应的入口
HANDLE_ENTRY( 0x21, ZERO );     // 键盘中断对应的入口
HANDLE_ENTRY( 0x22, ZERO );     // 级联用的
HANDLE_ENTRY( 0x23, ZERO );     // 串口2对应的入口
HANDLE_ENTRY( 0x24, ZERO );     // 串口1对应的入口
HANDLE_ENTRY( 0x25, ZERO );     // 并口2对应的入口
HANDLE_ENTRY( 0x26, ZERO );     // 软盘对应的入口
HANDLE_ENTRY( 0x27, ZERO );     // 并口1对应的入口
HANDLE_ENTRY( 0x28, ZERO );     // 实时时钟对应的入口
HANDLE_ENTRY( 0x29, ZERO );     // 重定向
HANDLE_ENTRY( 0x2a, ZERO );     // 保留
HANDLE_ENTRY( 0x2b, ZERO );     // 保留
HANDLE_ENTRY( 0x2c, ZERO );     // ps/2鼠标
HANDLE_ENTRY( 0x2d, ZERO );     // x87浮点单元异常
HANDLE_ENTRY( 0x2e, ZERO );     // 硬盘
HANDLE_ENTRY( 0x2f, ZERO );     // 保留
HANDLE_ENTRY( 0x30, ZERO );
HANDLE_ENTRY( 0x31, ZERO );
HANDLE_ENTRY( 0x32, ZERO );
HANDLE_ENTRY( 0x33, ZERO );
HANDLE_ENTRY( 0x34, ZERO );
HANDLE_ENTRY( 0x35, ZERO );
HANDLE_ENTRY( 0x36, ZERO );
HANDLE_ENTRY( 0x37, ZERO );
HANDLE_ENTRY( 0x38, ZERO );
HANDLE_ENTRY( 0x39, ZERO );
HANDLE_ENTRY( 0x3a, ZERO );
HANDLE_ENTRY( 0x3b, ZERO );
HANDLE_ENTRY( 0x3c, ZERO );
HANDLE_ENTRY( 0x3d, ZERO );
HANDLE_ENTRY( 0x3e, ZERO );
HANDLE_ENTRY( 0x3f, ZERO );
HANDLE_ENTRY( 0x40, ZERO );     // 系统调用
HANDLE_ENTRY( 0x41, ZERO );
HANDLE_ENTRY( 0x42, ZERO );
HANDLE_ENTRY( 0x43, ZERO );
HANDLE_ENTRY( 0x44, ZERO );
HANDLE_ENTRY( 0x45, ZERO );
HANDLE_ENTRY( 0x46, ZERO );
HANDLE_ENTRY( 0x47, ZERO );
HANDLE_ENTRY( 0x48, ZERO );
HANDLE_ENTRY( 0x49, ZERO );
HANDLE_ENTRY( 0x4a, ZERO );
HANDLE_ENTRY( 0x4b, ZERO );
HANDLE_ENTRY( 0x4c, ZERO );
HANDLE_ENTRY( 0x4d, ZERO );
HANDLE_ENTRY( 0x4e, ZERO );
HANDLE_ENTRY( 0x4f, ZERO );
HANDLE_ENTRY( 0x50, ZERO );
HANDLE_ENTRY( 0x51, ZERO );
HANDLE_ENTRY( 0x52, ZERO );
HANDLE_ENTRY( 0x53, ZERO );
HANDLE_ENTRY( 0x54, ZERO );
HANDLE_ENTRY( 0x55, ZERO );
HANDLE_ENTRY( 0x56, ZERO );
HANDLE_ENTRY( 0x57, ZERO );
HANDLE_ENTRY( 0x58, ZERO );
HANDLE_ENTRY( 0x59, ZERO );
HANDLE_ENTRY( 0x5a, ZERO );
HANDLE_ENTRY( 0x5b, ZERO );
HANDLE_ENTRY( 0x5c, ZERO );
HANDLE_ENTRY( 0x5d, ZERO );
HANDLE_ENTRY( 0x5e, ZERO );
HANDLE_ENTRY( 0x5f, ZERO );
HANDLE_ENTRY( 0x60, ZERO );
HANDLE_ENTRY( 0x61, ZERO );
HANDLE_ENTRY( 0x62, ZERO );
HANDLE_ENTRY( 0x63, ZERO );
HANDLE_ENTRY( 0x64, ZERO );
HANDLE_ENTRY( 0x65, ZERO );
HANDLE_ENTRY( 0x66, ZERO );
HANDLE_ENTRY( 0x67, ZERO );
HANDLE_ENTRY( 0x68, ZERO );
HANDLE_ENTRY( 0x69, ZERO );
HANDLE_ENTRY( 0x6a, ZERO );
HANDLE_ENTRY( 0x6b, ZERO );
HANDLE_ENTRY( 0x6c, ZERO );
HANDLE_ENTRY( 0x6d, ZERO );
HANDLE_ENTRY( 0x6e, ZERO );
HANDLE_ENTRY( 0x6f, ZERO );
HANDLE_ENTRY( 0x70, ZERO );
HANDLE_ENTRY( 0x71, ZERO );
HANDLE_ENTRY( 0x72, ZERO );
HANDLE_ENTRY( 0x73, ZERO );
HANDLE_ENTRY( 0x74, ZERO );
HANDLE_ENTRY( 0x75, ZERO );
HANDLE_ENTRY( 0x76, ZERO );
HANDLE_ENTRY( 0x77, ZERO );
HANDLE_ENTRY( 0x78, ZERO );
HANDLE_ENTRY( 0x79, ZERO );
HANDLE_ENTRY( 0x7a, ZERO );
HANDLE_ENTRY( 0x7b, ZERO );
HANDLE_ENTRY( 0x7c, ZERO );
HANDLE_ENTRY( 0x7d, ZERO );
HANDLE_ENTRY( 0x7e, ZERO );
HANDLE_ENTRY( 0x7f, ZERO );
HANDLE_ENTRY( 0x80, ZERO );
HANDLE_ENTRY( 0x81, ZERO );
HANDLE_ENTRY( 0x82, ZERO );
HANDLE_ENTRY( 0x83, ZERO );
HANDLE_ENTRY( 0x84, ZERO );
HANDLE_ENTRY( 0x85, ZERO );
HANDLE_ENTRY( 0x86, ZERO );
HANDLE_ENTRY( 0x87, ZERO );
HANDLE_ENTRY( 0x88, ZERO );
HANDLE_ENTRY( 0x89, ZERO );
HANDLE_ENTRY( 0x8a, ZERO );
HANDLE_ENTRY( 0x8b, ZERO );
HANDLE_ENTRY( 0x8c, ZERO );
HANDLE_ENTRY( 0x8d, ZERO );
HANDLE_ENTRY( 0x8e, ZERO );
HANDLE_ENTRY( 0x8f, ZERO );
HANDLE_ENTRY( 0x90, ZERO );
HANDLE_ENTRY( 0x91, ZERO );
HANDLE_ENTRY( 0x92, ZERO );
HANDLE_ENTRY( 0x93, ZERO );
HANDLE_ENTRY( 0x94, ZERO );
HANDLE_ENTRY( 0x95, ZERO );
HANDLE_ENTRY( 0x96, ZERO );
HANDLE_ENTRY( 0x97, ZERO );
HANDLE_ENTRY( 0x98, ZERO );
HANDLE_ENTRY( 0x99, ZERO );
HANDLE_ENTRY( 0x9a, ZERO );
HANDLE_ENTRY( 0x9b, ZERO );
HANDLE_ENTRY( 0x9c, ZERO );
HANDLE_ENTRY( 0x9d, ZERO );
HANDLE_ENTRY( 0x9e, ZERO );
HANDLE_ENTRY( 0x9f, ZERO );
HANDLE_ENTRY( 0xa0, ZERO );
HANDLE_ENTRY( 0xa1, ZERO );
HANDLE_ENTRY( 0xa2, ZERO );
HANDLE_ENTRY( 0xa3, ZERO );
HANDLE_ENTRY( 0xa4, ZERO );
HANDLE_ENTRY( 0xa5, ZERO );
HANDLE_ENTRY( 0xa6, ZERO );
HANDLE_ENTRY( 0xa7, ZERO );
HANDLE_ENTRY( 0xa8, ZERO );
HANDLE_ENTRY( 0xa9, ZERO );
HANDLE_ENTRY( 0xaa, ZERO );
HANDLE_ENTRY( 0xab, ZERO );
HANDLE_ENTRY( 0xac, ZERO );
HANDLE_ENTRY( 0xad, ZERO );
HANDLE_ENTRY( 0xae, ZERO );
HANDLE_ENTRY( 0xaf, ZERO );
HANDLE_ENTRY( 0xb0, ZERO );
HANDLE_ENTRY( 0xb1, ZERO );
HANDLE_ENTRY( 0xb2, ZERO );
HANDLE_ENTRY( 0xb3, ZERO );
HANDLE_ENTRY( 0xb4, ZERO );
HANDLE_ENTRY( 0xb5, ZERO );
HANDLE_ENTRY( 0xb6, ZERO );
HANDLE_ENTRY( 0xb7, ZERO );
HANDLE_ENTRY( 0xb8, ZERO );
HANDLE_ENTRY( 0xb9, ZERO );
HANDLE_ENTRY( 0xba, ZERO );
HANDLE_ENTRY( 0xbb, ZERO );
HANDLE_ENTRY( 0xbc, ZERO );
HANDLE_ENTRY( 0xbd, ZERO );
HANDLE_ENTRY( 0xbe, ZERO );
HANDLE_ENTRY( 0xbf, ZERO );
HANDLE_ENTRY( 0xc0, ZERO );
HANDLE_ENTRY( 0xc1, ZERO );
HANDLE_ENTRY( 0xc2, ZERO );
HANDLE_ENTRY( 0xc3, ZERO );
HANDLE_ENTRY( 0xc4, ZERO );
HANDLE_ENTRY( 0xc5, ZERO );
HANDLE_ENTRY( 0xc6, ZERO );
HANDLE_ENTRY( 0xc7, ZERO );
HANDLE_ENTRY( 0xc8, ZERO );
HANDLE_ENTRY( 0xc9, ZERO );
HANDLE_ENTRY( 0xca, ZERO );
HANDLE_ENTRY( 0xcb, ZERO );
HANDLE_ENTRY( 0xcc, ZERO );
HANDLE_ENTRY( 0xcd, ZERO );
HANDLE_ENTRY( 0xce, ZERO );
HANDLE_ENTRY( 0xcf, ZERO );
HANDLE_ENTRY( 0xd0, ZERO );
HANDLE_ENTRY( 0xd1, ZERO );
HANDLE_ENTRY( 0xd2, ZERO );
HANDLE_ENTRY( 0xd3, ZERO );
HANDLE_ENTRY( 0xd4, ZERO );
HANDLE_ENTRY( 0xd5, ZERO );
HANDLE_ENTRY( 0xd6, ZERO );
HANDLE_ENTRY( 0xd7, ZERO );
HANDLE_ENTRY( 0xd8, ZERO );
HANDLE_ENTRY( 0xd9, ZERO );
HANDLE_ENTRY( 0xda, ZERO );
HANDLE_ENTRY( 0xdb, ZERO );
HANDLE_ENTRY( 0xdc, ZERO );
HANDLE_ENTRY( 0xdd, ZERO );
HANDLE_ENTRY( 0xde, ZERO );
HANDLE_ENTRY( 0xdf, ZERO );
HANDLE_ENTRY( 0xe0, ZERO );
HANDLE_ENTRY( 0xe1, ZERO );
HANDLE_ENTRY( 0xe2, ZERO );
HANDLE_ENTRY( 0xe3, ZERO );
HANDLE_ENTRY( 0xe4, ZERO );
HANDLE_ENTRY( 0xe5, ZERO );
HANDLE_ENTRY( 0xe6, ZERO );
HANDLE_ENTRY( 0xe7, ZERO );
HANDLE_ENTRY( 0xe8, ZERO );
HANDLE_ENTRY( 0xe9, ZERO );
HANDLE_ENTRY( 0xea, ZERO );
HANDLE_ENTRY( 0xeb, ZERO );
HANDLE_ENTRY( 0xec, ZERO );
HANDLE_ENTRY( 0xed, ZERO );
HANDLE_ENTRY( 0xee, ZERO );
HANDLE_ENTRY( 0xef, ZERO );
HANDLE_ENTRY( 0xf0, ZERO );
HANDLE_ENTRY( 0xf1, ZERO );
HANDLE_ENTRY( 0xf2, ZERO );
HANDLE_ENTRY( 0xf3, ZERO );
HANDLE_ENTRY( 0xf4, ZERO );
HANDLE_ENTRY( 0xf5, ZERO );
HANDLE_ENTRY( 0xf6, ZERO );
HANDLE_ENTRY( 0xf7, ZERO );
HANDLE_ENTRY( 0xf8, ZERO );
HANDLE_ENTRY( 0xf9, ZERO );
HANDLE_ENTRY( 0xfa, ZERO );
HANDLE_ENTRY( 0xfb, ZERO );
HANDLE_ENTRY( 0xfc, ZERO );
HANDLE_ENTRY( 0xfd, ZERO );
HANDLE_ENTRY( 0xfe, ZERO );
HANDLE_ENTRY( 0xff, ZERO );
#undef HANDLE_ENTRY

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
        ( 1 << 3 ),                                          \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::TRAP ) )
/**
 * @brief 设置中断入口（System）
 */
#define SET_SYSTEM_HANDLER( index, ist )                     \
    this->make_descriptor(                                   \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        ( 1 << 3 ),                                          \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::SYSTEM ) )
/**
 * @brief 设置中断入口（System——Interrupt）
 */
#define SET_SYSTEM_INTERRUPT_HANDLER( index, ist )           \
    this->make_descriptor(                                   \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        ( 1 << 3 ),                                          \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::SYSTEM_INTERRUPT ) )
/**
 * @brief 设置中断入口（Interrupt）
 */
#define SET_INTERRUPT_HANDLER( index, ist )                  \
    this->make_descriptor(                                   \
        index,                                               \
        ( Lib::Types::uint64_t )( InterruptHandler##index ), \
        ( 1 << 3 ),                                          \
        ist,                                                 \
        static_cast< Lib::Types::uint8_t >( InterruptDescriptorAttribute::INTERRUPT ) )

PUBLIC namespace QuantumNEC::Architecture::CPU {
    PUBLIC using namespace Lib::IO;
    PRIVATE Lib::Types::Ptr< CONST Lib::Types::char_t > interrupt_name[ Platform::INTERRUPT_DESCRIPTOR_COUNT ];     // 各个异常的名字
    _C_LINK auto general_interrupt_handler( Lib::Types::Ptr< CONST InterruptFrame > frame )->ASMCALL VOID {
        using namespace QuantumNEC::Lib;
        using namespace QuantumNEC::Lib::IO;
#ifndef APIC
        if ( frame->irq == 0x27 || frame->irq == 0x2f )     // 0x2f是从片8259A上的最后一个irq引脚，保留
            return;                                         // IRQ7和IRQ15会产生伪中断(spurious interrupt),无须处理。
#endif
        if ( InterruptDescriptorManagement::interrupt_entry_table[ frame->irq ] == reinterpret_cast< Types::Ptr< VOID > >( general_interrupt_handler ) ) {
            // 显示中断名
            sout[ ostream::HeadLevel::ERROR ] << "IRQ_" << frame->irq << ":" << QuantumNEC::Architecture::CPU::interrupt_name[ frame->irq ] << endl;
            // 显示各个寄存器值
            sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "Rflags -> %#018lx\n\r", frame->rflags );
            sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RIP -> %#018lx\tRSP -> %#018lx\tSS  -> %#018lx\tCS  -> %#018lx\n\r", frame->rip, frame->rsp, frame->ss, frame->cs );
            ::QuantumNEC::Architecture::CPU::InterruptDescriptorManagement::display_registers( &frame->regs );
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
                Utils::hlt( );
        }
        else {
            InterruptDescriptorManagement::interrupt_entry_table[ frame->irq ]( frame );
            return;
        }
    }
    InterruptDescriptorManagement::InterruptDescriptorManagement( IN Lib::Types::Ptr< InterruptDescriptor > _descriptor, IN Lib::Types::uint16_t _num ) :
        Platform::DescriptorManagement< InterruptDescriptor > {
            _descriptor, _num
        } {
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the interrupt descriptor table management." << Lib::IO::endl;
        Utils::cli( );     // 关中断
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
        entry->offset_middle = ( entry_point >> 16 ) & 0xffff;
        entry->offset_high = ( entry_point >> 32 ) & 0xffffffff;
        return *entry;
    }
    auto InterruptDescriptorManagement::display_registers( IN Lib::Types::Ptr< CONST Platform::RegisterFrame > registers )->VOID {
        using namespace ::QuantumNEC::Lib::IO;
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RAX -> %#018lx\tRBX -> %#018lx\tRCX -> %#018lx\tRDX -> %#018lx\n\r", registers->rax, registers->rbx, registers->rcx, registers->rdx );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "R8  -> %#018lx\tR9  -> %#018lx\tR10 -> %#018lx\tR11 -> %#018lx\tR12 -> %#018lx\tR13 -> %#018lx\tR14 -> %#018lx\tR15 -> %#018lx\n\r", registers->r8, registers->r9, registers->r10, registers->r11, registers->r12, registers->r13, registers->r14, registers->r15 );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "RDI -> %#018lx\tRSI -> %#018lx\tRBP -> %#018lx\n\r", registers->rdi, registers->rsi, registers->rbp );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "CR0 -> %#018lx\tCR2 -> %#018lx\tCR3 -> %#018lx\tCR4 -> %#018lx\n\r", registers->cr0, registers->cr2, registers->cr3, registers->cr4 );
        sout[ ostream::HeadLevel::INFO ].printk( DisplayColor::WHITE, DisplayColor::BLACK, "DS -> %#018lx\tES -> %#018lx\tFS -> %#018lx\tGS -> %#018lx\n\r", registers->ds, registers->es, registers->fs, registers->gs );
        return;
    }
    auto InterruptDescriptorManagement::set_interrupt_handler( IN CONST Lib::Types::uint8_t irq, IN Lib::Types::FuncPtr< VOID, Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > > handle )->VOID {
        if ( irq > 0x1d ) {
            interrupt_entry_table[ irq ] = handle;
        }
        return;
    }
    auto InterruptDescriptorManagement::set_exception_handler( IN CONST Lib::Types::uint8_t irq, IN Lib::Types::FuncPtr< VOID, Lib::Types::Ptr< CONST Architecture::CPU::InterruptFrame > > handle )->VOID {
        if ( irq < 0x1e ) {
            interrupt_entry_table[ irq ] = handle;
        }
        return;
    }
    GlobalSegmentDescriptorManagement::GlobalSegmentDescriptorManagement( IN GlobalSegmentDescriptor _descriptor[ Platform::GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT ][ Platform::SEGMENT_DESCRIPTOR_COUNT ], IN Lib::Types::uint16_t _size ) :
        DescriptorManagement< GlobalSegmentDescriptor > {
            _descriptor[ 0 ], _size
        } {
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the global descriptor table management." << Lib::IO::endl;
        // 清零
        for ( Lib::Types::uint64_t i { 1ull }; i < Platform::GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT; ++i ) {
            Lib::STL::memset( &_descriptor[ i ], 0, sizeof *_descriptor );
        }
        sout[ ostream::HeadLevel::START ] << "Initializethe the task state segment." << endl;
        // 设置GDT里的LDT
        for ( Lib::Types::size_t i { }; i < Platform::GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT; ++i ) {
            make_descriptor( _descriptor[ i ], 0, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0, 0 );
            make_descriptor( _descriptor[ i ], 1, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0, Platform::AR_CODE64 );
            make_descriptor( _descriptor[ i ], 2, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0, Platform::AR_DATA64 );
            make_descriptor( _descriptor[ i ], 3, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0xfffff, Platform::AR_CODE32_DPL3 );
            make_descriptor( _descriptor[ i ], 4, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0xfffff, Platform::AR_DATA32_DPL3 );
            make_descriptor( _descriptor[ i ], 5, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0, Platform::AR_CODE64_DPL3 );
            make_descriptor( _descriptor[ i ], 6, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0, Platform::AR_DATA64_DPL3 );
            make_descriptor( _descriptor[ i ], 7, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0xfffff, Platform::AR_CODE32 );
            make_descriptor( _descriptor[ i ], 8, static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::SYSTEM ), 0xfffff, Platform::AR_DATA32 );
        }
        // 设置GDT里的TSS
        sout[ ostream::HeadLevel::SYSTEM ] << "Setting the task state segment descriptor." << endl;
        for ( Lib::Types::size_t i { }, tss_base_low { }, tss_base_high { }; i < Platform::TASK_STATE_SEGMENT_DESCRIPTOR_COUNT; ++i ) {
            Lib::STL::memset( &this->tss[ i ], 0, sizeof( TaskStateSegmentDescriptor ) );
            this->tss[ i ].io_map_base_address = static_cast< Lib::Types::uint16_t >( sizeof( TaskStateSegmentDescriptor ) << 16 );
            tss_base_low = ( reinterpret_cast< Lib::Types::uint64_t >( &this->tss[ i ] ) ) & 0xffffffff;
            tss_base_high = ( reinterpret_cast< Lib::Types::uint64_t >( &this->tss[ i ] ) >> 32 ) & 0xffffffff;
            GlobalSegmentDescriptorManagement::make_descriptor( _descriptor[ i ], 9, static_cast< Lib::Types::uint64_t >( tss_base_low & 0xffffffff ), sizeof( TaskStateSegmentDescriptor ) - static_cast< Lib::Types::uint64_t >( GlobalSegmentDescriptorType::NORMAL ), Platform::AR_TSS64 );
            Lib::STL::memcpy( &_descriptor[ i ][ 10 ], &tss_base_high, 8 );
        }
        sout[ ostream::HeadLevel::OK ] << "Initialize the task state segment." << endl;
        this->load( );
        // 刷新TSS
        ASM(
            "movw $0, %%ax \n\t"
            "movw %%ax,%%ds \n\t"
            "movw %%ax,%%es \n\t"
            "movw %%ax,%%fs \n\t"
            "movw %%ax,%%gs \n\t"
            "movw %%ax,%%ss \n\t"
            "pushq %[SELECTOR_CODE64] \n\t"
            "leaq .1(%%rip),%%rax \n\t"
            "pushq %%rax \n\t"
            "lretq \n\r"
            ".1: \n\t"
            "ltr %w[TSS] \n\t"
            :
            : [SELECTOR_CODE64] "i"( Platform::SELECTOR_CODE64_KERNEL ),
              [SELECTOR_DATA64] "ax"( Platform::SELECTOR_DATA64_KERNEL ),
              [TSS] "r"( Platform::SELECTOR_TSS )
            : );
        sout[ ostream::HeadLevel::SYSTEM ] << "Loading the global segment descriptor table." << endl;

        g_tss = (void *)this->tss;

        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the global descriptor table management." << Lib::IO::endl;
    }
    auto GlobalSegmentDescriptorManagement::load( VOID ) CONST->VOID {
        ASM( "lgdt %0" ::"m"( this->xdtr ) );
        return;
    }
    auto GlobalSegmentDescriptorManagement::read( VOID ) CONST->Lib::Types::Ptr< GlobalSegmentDescriptor > {
        ASM( "sgdt %0" ::"m"( this->xdtr ) );
        return this->xdtr.descriptor;
    }
    auto GlobalSegmentDescriptorManagement::make_descriptor( IN Lib::Types::Ptr< GlobalSegmentDescriptor > descriptor, IN Lib::Types::uint8_t index, IN Lib::Types::uint64_t base, IN Lib::Types::uint64_t limit, IN Lib::Types::uint64_t access )->Lib::Types::L_Ref< GlobalSegmentDescriptor > {
        Lib::Types::Ptr< GlobalSegmentDescriptor > entry { &descriptor[ index ] };
        // 分别设置描述符
        entry->limit_low = limit & 0xffff;
        entry->base_low = base & 0xffff;
        entry->base_middle = ( base & 0xff0000 ) >> 16;
        entry->access_right = access & 0xff;                                            // TYPE,S,DPL,P
        entry->limit_high = ( ( limit >> 16 ) & 0xf ) | ( ( access >> 8 ) & 0xf0 );     // AVL,L,D/B,G
        entry->base_high = ( base >> 24 ) & 0xff;
        return *entry;
    }
}
