#include <Kernel/Memory/map.hpp>
#include <Kernel/Memory/heap.hpp>
#include <Kernel/Memory/paging.hpp>
#include <Arch/Arch.hpp>
#include <Lib/IO/Stream/iostream>
#include <Lib/STL/string>
#include <Driver/driver.hpp>
#include <Kernel/task.hpp>
PUBLIC namespace {
    using namespace QuantumNEC::Lib::Types;
    using namespace QuantumNEC::Lib;
    using namespace QuantumNEC;
    /**
     *  @brief 页内偏移
     */
    /**
     *  @brief 通过虚拟地址查找条目或其他条目
     */
    PRIVATE constexpr auto PMLE_IDX( auto address ) {
        return ( (Lib::Types::uint64_t)address >> 39 ) & 0x1FF;
    }
    PRIVATE constexpr auto PDPTE_IDX( auto address ) {
        return ( (Lib::Types::uint64_t)address >> 30 ) & 0x1FF;
    }
    PRIVATE constexpr auto PDE_IDX( auto address ) {
        return ( (Lib::Types::uint64_t)address >> 21 ) & 0x1FF;
    }
    PRIVATE constexpr auto PTE_IDX( auto address ) {
        return ( (Lib::Types::uint64_t)address >> 12 ) & 0x1FF;
    }
    PRIVATE constexpr auto FONT_FILE_OCCUPIES_PAGE { 16 };
}
PUBLIC namespace QuantumNEC::Kernel::Memory {
    STATIC Task::TaskLock lock { };
    auto MemoryMapManagement::get_table_entry( IN Lib::Types::Ptr< VOID > address, IN MapLevel level )
        ->Lib::Types::Ptr< Lib::Types::uint64_t > {
        auto get_pmlt_entry = [ &, this ] {
            return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( ( Lib::Types::uint64_t )( this->page_memory_table.pml ) + PMLE_IDX( address ) );
        };
        auto get_pdpt_entry = [ &, this ] {
            return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( ( *get_pmlt_entry( ) & ~0xfff ) + PDPTE_IDX( address ) );
        };
        auto get_pd_entry = [ &, this ] {
            return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( ( *get_pdpt_entry( ) & ~0xfff ) + PDE_IDX( address ) );
        };
        Lib::Types::Ptr< Lib::Types::uint64_t > entry { };
        using enum MapLevel;
        switch ( level ) {
        case PML:
            entry = get_pmlt_entry( );
            break;
        case PDPT:
            entry = get_pdpt_entry( );
            break;
        case PD:
            entry = get_pd_entry( );
            break;
        case PT:
            entry = reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( ( *get_pd_entry( ) & ~0xfff ) + Lib::Base::OFFSET( address ) );
            break;
        default:
            entry = NULL;
            break;
        }
        return entry;
    }
    MemoryMapManagement::MemoryMapManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept {
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Start mapping the page table." << Lib::IO::endl;
        // 消除页保护
        this->page_table_protect( FALSE );
        // 获取在启动时服务设置的页表
        this->page_memory_table.pml = this->get_current_page_tabel( );
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::INFO ] << "Get a pages table from cr3, address of page table -> " << (VOID *)( this->page_memory_table.pml ) << "." << Lib::IO::endl;
        // 映射
        /*
          系统内存分配:
          · 0x0000000 -> 0x01fffff = [2MB]  --> 内核
          · 0x0200000 -> 0x020ffff = [64KB] --> 内核PCB
          · 0x0210000 -> 0x0210fff = [4KB]  --> I-O APIC 所占用
          · 0x0211000 -> 0x0211fff = [4KB]  --> Local APIC 所占用
          · 0x0212000 -> 0x05fffff = [4MB]  --> 内核保留
          · 0x0600000 -> 0x07fffff = [2MB]  --> 内核页表
          · 0x0800000 -> 0x09fffff = [2MB]  --> 显存
          · 0x0a00000 -> 0x29fffff = [32MB] --> 字体文件
          · 0x2a00000 -> .........          --> 空闲
          4GB 以下显存按照这样分配
          0x0000000000000000 - 0x00000000ffffffff ==> 0x0000000000000000 - 0x00000000ffffffff （也就是不映射）
          4GB 以上显存按这样分配（映射，从线性地址0xffff800000000000开始）
          0x0000000000000000 - 0x00000000ffffffff ==> 0xffff800000000000 - 0xffff8000ffffffff
        */

        // 映射内核起始地址
        this->map( 0, KERNEL_BASE_ADDRESS, 1, PAGE_KERNEL_DIR, MapMode::MEMORY_MAP_2M );
        // 映射内核PCB
        this->map( KERNEL_TASK_PCB_PHYSICAL_ADDRESS, KERNEL_TASK_PCB_VIRTUAL_ADDRESS, 21, PAGE_KERNEL_PAGE, MapMode::MEMORY_MAP_4K );
        // 映射内核IO apic与Local apic所占用内存
        this->map( reinterpret_cast< Lib::Types::uint64_t >( Driver::DriverManagement::io_apic_address ), KERNEL_I_O_APIC_VIRTUAL_ADDRESS, 1, PAGE_KERNEL_DIR, MapMode::MEMORY_MAP_4K );
        this->map( reinterpret_cast< Lib::Types::uint64_t >( Driver::DriverManagement::local_apic_address ), KERNEL_LOCAL_APIC_VIRTUAL_ADDRESS, 1, PAGE_KERNEL_DIR, MapMode::MEMORY_MAP_4K );
        // 映射内核根目录
        this->map( reinterpret_cast< Lib::Types::uint64_t >( this->page_memory_table.pml ), KERNEL_PAGE_DIRECTORY_VIRTUAL_ADDRESS, 1, PAGE_KERNEL_PAGE, MapMode::MEMORY_MAP_2M );
        // 映射显存
        this->map( _config->GraphicsData.FrameBufferBase, KERNEL_VRAM_VIRTUAL_ADDRESS, 1, PAGE_KERNEL_PAGE, MapMode::MEMORY_MAP_2M );
        // 字体文件映射到的内存缓存池
        Lib::Types::uint64_t font_table_buffer { reinterpret_cast< Lib::Types::uint64_t >( PageMemoryManagement::malloc( FONT_FILE_OCCUPIES_PAGE, PageMemoryManagement::MemoryPageType::PAGE_2M ) ) };
        // 映射字体
        this->map( font_table_buffer, KERNEL_FONT_MEMORY_VIRTUAL_ADDRESS, FONT_FILE_OCCUPIES_PAGE, PAGE_KERNEL_PAGE, MapMode::MEMORY_MAP_2M );
        // 映射空闲内存
        for ( Lib::Types::size_t page_address { KERNEL_FREE_MEMORY_PHYSICAL_ADDRESS }; page_address < PageMemoryManagement::memory_total; page_address += PAGE_SIZE ) {
            // 从空闲内存起始地址开始映射
            this->map( page_address, KERNEL_BASE_ADDRESS + page_address, 1, PAGE_USER_PAGE, MapMode::MEMORY_MAP_2M );
        }

        Architecture::ArchitectureManagement< TARGET_ARCH >::flush_tlb( );
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Mapping the pages table is OK." << Lib::IO::endl;
    }
    auto MemoryMapManagement::map( IN Lib::Types::uint64_t physics_address, IN Lib::Types::uint64_t virtual_address, IN Lib::Types::size_t size, IN Lib::Types::uint16_t flags, IN MapMode mode, IN Lib::Types::Ptr< Lib::Types::uint64_t > pml )->VOID {
        lock.acquire( );
        STATIC Lib::Types::uint64_t pd_reserved_memory { KERNEL_PAGE_TABLE_RESERVED_PHYSICAL_ADDRESS };
        auto checkMode = [ & ] -> Lib::Types::uint32_t {     // 解析格式
            return mode == MapMode::MEMORY_MAP_2M ? MEMORY_SIZE_2M : ( mode == MapMode::MEMORY_MAP_1G ? MEMORY_SIZE_1G : MEMORY_SIZE_4K );
        };
        physics_address = physics_address & ~( PAGE_SIZE * size - 1 );
        virtual_address = virtual_address & ~( PAGE_SIZE * size - 1 );
        pml_t virtual_pml { }, virtual_pmle { };
        pdpt_t virtual_pdpt { }, physical_pdpt { }, virtual_pdpte { }, physical_pdpte { };
        pdt_t virtual_pdt { }, physical_pdt { }, virtual_pde { }, physical_pde { };
        virtual_pml.set_mplt( pml );
        virtual_pmle.set_mplt( virtual_pml.pml + PMLE_IDX( virtual_address ) );
        if ( !( *virtual_pmle.pml & PAGE_PRESENT ) ) {
            physical_pdpt.set_pdpt( reinterpret_cast< decltype( physical_pdpt.pdpt ) >( pd_reserved_memory += PT_SIZE ) );

            virtual_pdpt.set_pdpt( reinterpret_cast< decltype( virtual_pdpt.pdpt ) >( ( physical_pdpt.pdpt ) ) );
            Lib::STL::memset( virtual_pdpt.pdpt, 0, PT_SIZE );
            virtual_pmle.make_mplt( physical_pdpt.pdpt, flags );
        }
        physical_pdpt.set_pdpt( reinterpret_cast< decltype( physical_pdpt.pdpt ) >( *virtual_pmle.pml & ( ~0xfff ) ) );
        physical_pdpte.set_pdpt( physical_pdpt.pdpt + PDPTE_IDX( virtual_address ) );
        virtual_pdpte.set_pdpt( reinterpret_cast< decltype( virtual_pdpte.pdpt ) >( ( physical_pdpte.pdpt ) ) );
        if ( !( *virtual_pdpte.pdpt & PAGE_PRESENT ) ) {
            physical_pdt.set_pdt( reinterpret_cast< decltype( physical_pdt.pdt ) >( pd_reserved_memory += PT_SIZE ) );
            virtual_pdt.set_pdt( reinterpret_cast< decltype( virtual_pdt.pdt ) >( ( physical_pdt.pdt ) ) );
            Lib::STL::memset( virtual_pdt.pdt, 0, PT_SIZE );
            virtual_pdpte.make_pdpt( physical_pdt.pdt, flags );
        }
        physical_pdt.set_pdt( reinterpret_cast< decltype( physical_pdt.pdt ) >( *virtual_pdpte.pdpt & ( ~0xfff ) ) );
        physical_pde.set_pdt( physical_pdt.pdt + PDE_IDX( virtual_address ) );
        virtual_pde.set_pdt( reinterpret_cast< decltype( physical_pdt.pdt ) >( ( physical_pde.pdt ) ) );
        virtual_pde.make_pdt( reinterpret_cast< decltype( virtual_pde.pdt ) >( physics_address ), flags | checkMode( ) );
        Architecture::ArchitectureManagement< TARGET_ARCH >::invlpg( reinterpret_cast< Lib::Types::Ptr< VOID > >( virtual_address ) );
        lock.release( );
    }
    auto MemoryMapManagement::remap( IN Lib::Types::uint64_t virtual_address, IN Lib::Types::size_t size, IN Lib::Types::Ptr< Lib::Types::uint64_t > pml )->VOID {
        /*
         * 内容大致与map一至，除了取消映射的地方和判断
         */
        lock.acquire( );
        virtual_address = virtual_address & ~( PAGE_SIZE * size - 1 );
        pml_t virtual_pml { }, virtual_pmle { };
        pdpt_t physical_pdpt { }, virtual_pdpte { }, physical_pdpte { };
        pdt_t virtual_pde { }, physical_pdt { }, physical_pde { };
        virtual_pml.set_mplt( reinterpret_cast< decltype( virtual_pml.pml ) >( ( pml ) ) );
        virtual_pmle.set_mplt( virtual_pml.pml + PMLE_IDX( virtual_address ) );
        if ( !( *virtual_pmle.pml & PAGE_PRESENT ) ) {
            return;
        }
        physical_pdpt.set_pdpt( reinterpret_cast< decltype( physical_pdpt.pdpt ) >( *virtual_pmle.pml & ( ~0xFFF ) ) );
        physical_pdpte.set_pdpt( physical_pdpt.pdpt + PDPTE_IDX( virtual_address ) );
        virtual_pdpte.set_pdpt( reinterpret_cast< decltype( virtual_pdpte.pdpt ) >( ( physical_pdpte.pdpt ) ) );
        if ( !( *virtual_pdpte.pdpt & PAGE_PRESENT ) ) {
            return;
        }
        physical_pdt.set_pdt( reinterpret_cast< decltype( physical_pdt.pdt ) >( *virtual_pdpte.pdpt & ( ~0xFFF ) ) );
        physical_pde.set_pdt( physical_pdt.pdt + PDE_IDX( virtual_address ) );
        virtual_pde.set_pdt( reinterpret_cast< decltype( physical_pdt.pdt ) >( ( physical_pde.pdt ) ) );
        *virtual_pde.pdt &= ~PAGE_PRESENT;
        lock.release( );
    }
    auto MemoryMapManagement::check( IN Lib::Types::Ptr< Lib::Types::uint64_t > virtual_address )->Lib::Types::BOOL {
        if ( ( *virtual_address >> 47 ) & 1 ) {
            if ( *virtual_address >> 48 == 0xFFFF ) {
                return TRUE;
            }
            *virtual_address |= 0xFFFFULL << 48;
            return FALSE;
        }
        return TRUE;
    }
    auto MemoryMapManagement::page_table_protect( IN Lib::Types::BOOL flags )->VOID {
        if ( !flags ) {
            Architecture::ArchitectureManagement< TARGET_ARCH >::write_cr0( Architecture::ArchitectureManagement< TARGET_ARCH >::read_cr0( ) & ~0x10000 );
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ] << "Disable the page protection." << Lib::IO::endl;
        }
        else {
            Architecture::ArchitectureManagement< TARGET_ARCH >::write_cr0( Architecture::ArchitectureManagement< TARGET_ARCH >::read_cr0( ) | 0x10000 );
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ] << "Enable the page protection." << Lib::IO::endl;
        }
    };
    auto MemoryMapManagement::make_page_directory_table( VOID )->Lib::Types::Ptr< Lib::Types::uint64_t > {
        pml_t page_directory_address { .pml { reinterpret_cast< decltype( page_directory_address.pml ) >( HeapMemoryManagement::malloc( PT_SIZE ) ) } };

        if ( !page_directory_address.pml ) {
            return NULL;
        }

        Lib::STL::memcpy( page_directory_address.pml, page_memory_table.pml, 2048 );
        return page_directory_address.pml;
    }
    auto MemoryMapManagement::activate_page_directory_table( IN Lib::Types::Ptr< VOID > )->VOID {
        // Architecture::ArchitectureManagement< TARGET_ARCH >::write_cr3( page_directory_table_address ? reinterpret_cast< Lib::Types::uint64_t >( page_directory_table_address ) : reinterpret_cast< Lib::Types::uint64_t >( page_memory_table.pml ) );
    }
    auto MemoryMapManagement::get_current_page_tabel( VOID )->Lib::Types::Ptr< Lib::Types::uint64_t > {
        return reinterpret_cast< Lib::Types::Ptr< Lib::Types::uint64_t > >( Architecture::ArchitectureManagement< TARGET_ARCH >::read_cr3( ) );
    }
}