#include <Arch/x86_64/platform/platform.hpp>
#include <Lib/Base/deflib.hpp>
#include <Lib/IO/Stream/iostream>
#include <Utils/asm.hpp>

PUBLIC namespace QuantumNEC::Architecture::CPU {
    CPUManagement::CPUManagement( VOID ) :
        InterruptDescriptorManagement { idt, Architecture::Platform::GDT_MAX },
        GlobalDescriptorManagement { gdt, Architecture::Platform::IDT_MAX } {
        using namespace QuantumNEC::Lib::IO;
        using namespace QuantumNEC::Lib::Types;
        // CPU Management初始化
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the CPU Management." << Lib::IO::endl;
        using enum DisplayColor;
        uint32_t cpuFacName[ 4 ] { };
        char_t factoryName[ 17 ] { };
        this->cpuID( 0, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 0 ] ) = cpuFacName[ 1 ];
        *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 4 ] ) = cpuFacName[ 3 ];
        *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 8 ] ) = cpuFacName[ 2 ];
        factoryName[ 12 ] = '\0';
        sout[ ostream::HeadLevel::INFO ] << "CPU device information is as follows:" << endl;
        sout[ ostream::HeadLevel::SYSTEM ].printk( WHITE, BLACK, "%s\t%#010x\t%#010x\t%#010x\n", factoryName, cpuFacName[ 1 ], cpuFacName[ 3 ], cpuFacName[ 2 ] );
        // 获取处理器商标信息
        sout[ ostream::HeadLevel::SYSTEM ];
        for ( uint32_t i { 0x80000002 }; i <= 0x80000004; i++ ) {
            this->cpuID( i, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
            *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 0 ] ) = cpuFacName[ 0 ];
            *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 4 ] ) = cpuFacName[ 1 ];
            *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 8 ] ) = cpuFacName[ 2 ];
            *reinterpret_cast< Ptr< uint32_t > >( &factoryName[ 12 ] ) = cpuFacName[ 3 ];
            factoryName[ 16 ] = '\0';
            sout.printk( WHITE, BLACK, "%s", factoryName );
        }
        endl( sout );
        // 获取处理器的版本信息
        this->cpuID( 1, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        sout[ ostream::HeadLevel::SYSTEM ].printk( WHITE, BLACK,
                                                   "Family Code -> %#010x, Extended Family -> %#010x | Model Number:%#010x | Extended Model -> %#010x | Processor Type -> %#010x | Stepping ID -> %#010x\n",
                                                   ( cpuFacName[ 0 ] >> 8 & 0xf ),       // 处理器商标信息引索值(IA-32处理器特有)
                                                   ( cpuFacName[ 0 ] >> 20 & 0xff ),     // CLFLUSH指令刷新的缓存行容量(单位8B)
                                                   ( cpuFacName[ 0 ] >> 4 & 0xf ),       // 处理器包内最大可寻址逻辑处理器ID
                                                   ( cpuFacName[ 0 ] >> 16 & 0xf ),      // 初始 APIC ID
                                                   ( cpuFacName[ 0 ] >> 12 & 0x3 ),
                                                   ( cpuFacName[ 0 ] & 0xf ) );     // 上面两个都是处理器支持的位功能信息
        // 获取线性/物理地址位宽
        this->cpuID( 0x80000008, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        sout[ ostream::HeadLevel::SYSTEM ].printk( WHITE, BLACK, "Physical Address size -> %08d | Linear Address size -> %08d\n", ( cpuFacName[ 0 ] & 0xff ), ( cpuFacName[ 0 ] >> 8 & 0xff ) );
        // 获得处理器支持的最大基础功能号
        this->cpuID( 0, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        sout[ ostream::HeadLevel::SYSTEM ].printk( WHITE, BLACK, "Max Basic Operation Code -> %#010x  | ", cpuFacName[ 0 ] );
        // 获得处理器支持的最大扩展功能号
        this->cpuID( 0x80000000, 0, &cpuFacName[ 0 ], &cpuFacName[ 1 ], &cpuFacName[ 2 ], &cpuFacName[ 3 ] );
        sout.printk( WHITE, BLACK, "Max Extended Operation Code -> %#010x\n", cpuFacName[ 0 ] );
    }
    auto CPUManagement::cpuID( IN Lib::Types::uint32_t mop, IN Lib::Types::uint32_t sop, IN Lib::Types::Ptr< Lib::Types::uint32_t > eax, IN Lib::Types::Ptr< Lib::Types::uint32_t > ebx, IN Lib::Types::Ptr< Lib::Types::uint32_t > ecx, IN Lib::Types::Ptr< Lib::Types::uint32_t > edx )->VOID {
        ASM( "cpuid\n\t"
             : "=a"( *eax ), "=b"( *ebx ), "=c"( *ecx ), "=d"( *edx )
             : "0"( mop ), "2"( sop ) );
        return;
    }
}