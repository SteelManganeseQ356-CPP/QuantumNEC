#pragma once
#include <Arch/x86_64/cpu/xdt.hpp>
#include <Lib/Types/Uefi.hpp>
#include <Lib/Base/deflib.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    /**
     * @brief CPU管理类
     */
    PUBLIC class CPUs :
        public InterruptDescriptorManager,
        public GlobalSegmentDescriptorManager

    {
    public:
        struct CpuidStatus
        {
            Lib::Types::uint64_t mop;
            Lib::Types::uint64_t sop;
            Lib::Types::uint64_t rax;
            Lib::Types::uint64_t rbx;
            Lib::Types::uint64_t rcx;
            Lib::Types::uint64_t rdx;
        };

    public:
        explicit CPUs( VOID ) noexcept;
        virtual ~CPUs( VOID ) noexcept = default;

    public:
        STATIC auto cpuid( IN CpuidStatus &status ) -> CpuidStatus &;
        STATIC auto switch_to( IN OUT Lib::Types::Ptr< VOID > current, IN Lib::Types::Ptr< VOID > next ) -> VOID;
        STATIC auto port_insw( IN Lib::Types::uint64_t port, IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::uint64_t nr ) -> VOID;
        STATIC auto port_outsw( IN Lib::Types::uint64_t port, IN Lib::Types::Ptr< VOID > buffer, IN Lib::Types::uint64_t nr ) -> VOID;
        STATIC auto cli( VOID ) -> VOID;
        STATIC auto sti( VOID ) -> VOID;
        STATIC auto hlt( VOID ) -> VOID;
        STATIC auto nop( VOID ) -> VOID;
        STATIC auto rdmsr( IN Lib::Types::uint64_t address ) -> Lib::Types::uint64_t;
        STATIC auto wrmsr( IN Lib::Types::uint64_t address, IN Lib::Types::uint64_t value ) -> VOID;
        STATIC auto get_rsp( VOID ) -> Lib::Types::uint64_t;
        STATIC auto get_rflags( VOID ) -> Lib::Types::uint64_t;
        STATIC auto io_in8( IN Lib::Types::uint16_t port ) -> Lib::Types::uint8_t;
        STATIC auto io_in16( IN Lib::Types::uint16_t port ) -> Lib::Types::uint16_t;
        STATIC auto io_in32( IN Lib::Types::uint16_t port ) -> Lib::Types::uint32_t;
        STATIC auto io_out8( IN Lib::Types::uint16_t port, IN Lib::Types::uint8_t value ) -> VOID;
        STATIC auto io_out16( IN Lib::Types::uint16_t port, IN Lib::Types::uint16_t value ) -> VOID;
        STATIC auto io_out32( IN Lib::Types::uint16_t port, IN Lib::Types::uint32_t value ) -> VOID;
        STATIC auto read_cr15( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr15( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr14( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr14( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr13( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr13( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr12( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr12( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr11( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr11( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr10( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr10( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr9( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr9( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr8( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr8( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr7( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr7( IN Lib::Types::uint64_t cr2 ) -> VOID;
        STATIC auto read_cr6( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr6( IN Lib::Types::uint64_t cr0 ) -> VOID;
        STATIC auto read_cr5( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr5( IN Lib::Types::uint64_t cr0 ) -> VOID;
        STATIC auto read_cr4( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr4( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr3( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr3( IN Lib::Types::uint64_t cr3 ) -> VOID;
        STATIC auto read_cr2( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr2( IN Lib::Types::uint64_t cr2 ) -> VOID;
        STATIC auto read_cr1( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr1( IN Lib::Types::uint64_t cr0 ) -> VOID;
        STATIC auto read_cr0( VOID ) -> Lib::Types::uint64_t;
        STATIC auto write_cr0( IN Lib::Types::uint64_t cr0 ) -> VOID;
        STATIC auto invlpg( IN Lib::Types::Ptr< VOID > address ) -> VOID;
        STATIC auto pause( VOID ) -> VOID;
        STATIC auto mfence( VOID ) -> VOID;
        STATIC auto lfence( VOID ) -> VOID;
        STATIC auto sfence( VOID ) -> VOID;
        STATIC auto set_page_table( IN Lib::Types::Ptr< Lib::Types::uint64_t > mmap ) -> VOID;
        STATIC auto flush_tlb( VOID ) -> VOID;

    private:
        inline STATIC QuantumNEC::Architecture::GlobalSegmentDescriptor _used gdt[ GLOBAL_SEGMENT_DESCRIPTOR_TABLE_COUNT ][ SEGMENT_DESCRIPTOR_COUNT ] { };
        inline STATIC QuantumNEC::Architecture::InterruptDescriptor idt[ INTERRUPT_DESCRIPTOR_COUNT ] { };
    };

    // cpuid
}