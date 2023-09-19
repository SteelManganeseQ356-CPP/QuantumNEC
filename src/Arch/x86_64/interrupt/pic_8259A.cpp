#include <Arch/x86_64/platform/platform.hpp>
#include <Utils/asm.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt {
    Pic8259aManagement::Pic8259aManagement( VOID ) {
        Utils::io_out8( PIC_M_CTRL, 0x11 ); /* 边沿触发模式 */
        Utils::io_out8( PIC1_DATA, 0x20 );  /* IRQ0-7由INT20-27接收 */
        Utils::io_out8( PIC1_DATA, 0x04 );  /* PIC1由IRQ2连接*/
        Utils::io_out8( PIC1_DATA, 0x01 );  /* 无缓冲区模式 */
        Utils::io_out8( PIC_S_CTRL, 0x11 ); /* 与上方类似 */
        Utils::io_out8( PIC2_DATA, 0x28 );  /* IRQ8-15 INT28-2f */
        Utils::io_out8( PIC2_DATA, 0x02 );  /* PIC1 IRQ2 */
        Utils::io_out8( PIC2_DATA, 0x01 );  /* 无缓冲区模式 */
        Utils::io_out8( PIC1_DATA, 0xfc );
        Utils::io_out8( PIC2_DATA, 0xff );
    }
    Pic8259aManagement::~Pic8259aManagement( VOID ) {
    }
    auto Pic8259aManagement::eoi( IN CONST irq_t irq )->VOID {
        if ( irq >= 0x28 )
            Utils::io_out8( PIC_S_CTRL, PIC_EOI );
        Utils::io_out8( PIC_M_CTRL, PIC_EOI );
        return;
    }
}