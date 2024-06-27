
#include "Arch/x86_64/interrupt/8259a.hpp"
#include <Arch/x86_64/platform/platform.hpp>
#include <Kernel/memory.hpp>
#include <Lib/IO/Stream/iostream>

PUBLIC namespace QuantumNEC::Architecture {
    Apic::Apic( VOID ) noexcept {
        // 关闭8259A PIC
        PIC8259A::disable_8259A_pic( );
        // 开启APIC
        if ( this->check_apic( ) ) {
            Lib::Types::uint64_t apic_base_pa = CPUs::rdmsr( IA32_APIC_BASE_MSR );
            Lib::Types::Ptr< volatile Lib::Types::uint32_t > lapic = (Lib::Types::Ptr< volatile Lib::Types::uint32_t >)( apic_base_pa & IA32_APIC_BASE_MSR_BASE_ADDR_MSK );
            // LOCAL APIC

            // 启用本地 APIC;设置杂散中断向量。
            this->write_apic( LOCAL_APIC_SVR, LOCAL_APIC_ENABLE | LOCAL_APIC_SPURIOUS_INTERRUPTS_INDEX, ApicType::LOCAL_APIC );
            // 计时器在总线频率下反复倒计时
            // from lapic[TICR]，然后发出中断。
            // TODO：使用外部时间校准 TICR
            this->write_apic( LOCAL_APIC_TDCR, LOCAL_APIC_X1, ApicType::LOCAL_APIC );
            this->write_apic( LOCAL_APIC_TIMER, LOCAL_APIC_PERIODIC | CLOCK_INTERRUPTS_INDEX, ApicType::LOCAL_APIC );
            this->write_apic( LOCAL_APIC_TICR, 10000000, ApicType::LOCAL_APIC );
            // Disable logical interrupt lines.
            this->write_apic( LOCAL_APIC_LINT0, LOCAL_APIC_MASKED, ApicType::LOCAL_APIC );
            this->write_apic( LOCAL_APIC_LINT1, LOCAL_APIC_MASKED, ApicType::LOCAL_APIC );
            // 禁用性能计数器溢出中断
            // 在提供该中断条目的计算机上。
            if ( ( ( lapic[ LOCAL_APIC_VER ] >> 16 ) & 0xFF ) >= 4 )
                this->write_apic( LOCAL_APIC_PCINT, LOCAL_APIC_MASKED, ApicType::LOCAL_APIC );
            // 将错误中断映射到IRQ_ERROR。
            this->write_apic( LOCAL_APIC_ERROR, LOCAL_APIC_ERROR_INTERRUPTS_INDEX, ApicType::LOCAL_APIC );
            // 清除错误状态寄存器（需要背靠背写入）。
            this->write_apic( LOCAL_APIC_ESR, 0, ApicType::LOCAL_APIC );
            this->write_apic( LOCAL_APIC_ESR, 0, ApicType::LOCAL_APIC );
            // 确认任何未完成的中断。
            this->write_apic( LOCAL_APIC_EOI, 0, ApicType::LOCAL_APIC );
            // Send an Init Level De-Assert to synchronise arbitration ID's.
            this->write_apic( LOCAL_APIC_ICRHI, 0, ApicType::LOCAL_APIC );
            this->write_apic( LOCAL_APIC_ICRLO, LOCAL_APIC_BCAST | LOCAL_APIC_INIT | LOCAL_APIC_LEVEL, ApicType::LOCAL_APIC );
            while ( lapic[ LOCAL_APIC_ICRLO ] & LOCAL_APIC_DELIVS )
                ;
            // 发送 Init 级别取消断言以同步仲裁 ID。
            this->write_apic( LOCAL_APIC_TPR, 0, ApicType::LOCAL_APIC );
            // IO APIC
            // 将所有中断标记为边沿触发、高电平有效、禁用、并且不路由到任何 CPU。
            for ( Lib::Types::uint64_t i { }; i <= ( ( this->read_apic( IOAPIC_REG_VER, ApicType::IO_APIC ) >> 16 ) & 0xFF ); i++ ) {
                this->write_apic( IOAPIC_REG_TABLE + 2 * i, INT_DISABLED | ( IDT_ENTRY_IRQ_0 + i ), ApicType::IO_APIC );
                this->write_apic( IOAPIC_REG_TABLE + 2 * i + 1, 0, ApicType::IO_APIC );
            }
            ASM( "MOVQ %0, %%CR8" ::"r"( 0ull ) );
        }
        else {
            // 几乎没有意义
            // ACPI都过了你还检测个毛
            Lib::IO::sout << "CPU can not support apic!" << Lib::IO::endl;
        }
    }
    Apic::~Apic( VOID ) noexcept {
    }
    auto Apic::eoi( IN CONST irq_t )->VOID {
        write_apic( LOCAL_APIC_EOI, 0, ApicType::LOCAL_APIC );
    }
    auto Apic::check_apic( VOID )->Lib::Types::BOOL {
        Lib::Types::uint32_t eax { }, ebx { }, ecx { }, edx { };
        CPUs::cpuid( 1, 0, &eax, &ebx, &ecx, &edx );
        return ( edx & ( 1 << 9 ) ) && ( CPUs::rdmsr( IA32_APIC_BASE_MSR ) & ( 1 << 11 ) );
    }
    auto Apic::read_apic( IN Lib::Types::uint16_t index, IN ApicType type )->Lib::Types::uint64_t {
        if ( type == ApicType::LOCAL_APIC ) {
            return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint32_t volatile > >( apic.local_apic_address )[ index ];
            CPUs::mfence( );
        }
        else {
            Lib::Types::uint64_t ret { };
            *reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint8_t > >( apic.io_apic_index_address ) = index + 1;
            CPUs::mfence( );
            ret = *reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint8_t > >( apic.io_apic_data_address );
            ret <<= 32;
            CPUs::mfence( );
            *reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint8_t > >( apic.io_apic_index_address ) = index;
            CPUs::mfence( );
            ret |= *reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint8_t > >( apic.io_apic_data_address );
            CPUs::mfence( );
        }
        return 0;
    }
    auto Apic::write_apic( IN Lib::Types::uint16_t index, IN Lib::Types::uint32_t value, IN ApicType type )->VOID {
        if ( type == ApicType::LOCAL_APIC ) {
            reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint32_t volatile > >( apic.local_apic_address )[ index ] = value;
            CPUs::mfence( );
        }
        else {
            Lib::Types::uint64_t value_ { value };
            *reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint8_t > >( apic.io_apic_index_address ) = index;
            CPUs::mfence( );
            *reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint8_t > >( apic.io_apic_data_address ) = value_ & 0xffffffff;
            value_ >>= 32;
            CPUs::mfence( );
            *reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint8_t > >( apic.io_apic_index_address ) = index + 1;
            CPUs::mfence( );
            *reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint8_t > >( apic.io_apic_data_address ) = value_ & 0xffffffff;
            CPUs::mfence( );
        }
    }
    auto Apic::enable_ioapic( IN Lib::Types::uint64_t pin, IN Lib::Types::uint64_t vector )->VOID {
        Lib::Types::uint64_t value { read_apic( pin * 2 + 0x10, ApicType::IO_APIC ) };
        value = value & ~0x100ff;
        write_apic( pin * 2 + 0x10, value | vector, ApicType::IO_APIC );
    }
}