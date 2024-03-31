#include <Arch/Arch.hpp>
#include <Driver/driver.hpp>
#include <Kernel/kernel.hpp>
#include <Kernel/memory.hpp>
#include <Kernel/task.hpp>
#include <Lib/IO/Stream/iostream>
#include <Utils/asm.hpp>
#include <tuple>
using namespace QuantumNEC;
using namespace QuantumNEC::Lib;
using namespace QuantumNEC::Lib::Types;

//////////////////////////////////////分割线///////////////////////////////////////////////
/////////////////////////////////////以下为测试/////////////////////////////////////////////
Lib::Types::int64_t ThreadA( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Thread A : " << code << '\n';
    Lib::IO::sout << "PID:" << Kernel::TaskManagement::get_current< Kernel::TaskManagement::ThreadPCB >( )->PID << '\n';
    return 0;
}
Lib::Types::int64_t ThreadB( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Thread B : " << code << '\n';
    Lib::IO::sout << "PID:" << Kernel::TaskManagement::get_current< Kernel::TaskManagement::ThreadPCB >( )->PID << '\n';
    return 0;
}
Lib::Types::int64_t ProcC( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Process C : " << code << '\n';
    Lib::IO::sout << "PID:" << Kernel::TaskManagement::get_current< Kernel::TaskManagement::ProcessPCB >( )->PID << '\n';
    return 0;
}
Lib::Types::int64_t ProcD( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Process D : " << code << '\n';
    Lib::IO::sout << "PID:" << Kernel::TaskManagement::get_current< Kernel::TaskManagement::ProcessPCB >( )->PID << '\n';
    return 0;
}
Lib::Types::int64_t ProcE( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Process E : " << code << '\n';
    Lib::IO::sout << "PID:" << Kernel::TaskManagement::get_current< Kernel::TaskManagement::ProcessPCB >( )->PID << '\n';
    return 0;
}
//////////////////////////////////////分割线///////////////////////////////////////////////
_C_LINK auto micro_kernel_entry( IN Ptr< BootConfig > config ) -> SystemStatus {
    SystemStatus Status { SYSTEM_SUCCESS };
    Driver::DriverManagement { config };     // 驱动初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::INFO ] << "This is QuantumNEC-OS. Build number 10006." << Lib::IO::endl;
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the system architecture management." << Lib::IO::endl;
    Architecture::ArchitectureManagement< TARGET_ARCH > { config };     // 系统架构初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the system architecture management." << Lib::IO::endl;
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the memory management." << Lib::IO::endl;
    Kernel::MemoryManagement { config };     // 内存管理初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the memory management." << Lib::IO::endl;
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::START ] << "Initialize the task management." << Lib::IO::endl;
    Kernel::TaskManagement { config };     // 进程管理初始化
    Lib::IO::sout[ Lib::IO::ostream::HeadLevel::OK ] << "Initialize the task management." << Lib::IO::endl;
    Lib::IO::sout << "Test 1 : Memory allocate-------------------------------\n";
    char buf[] {
        "hello world\0"
    };
    auto w { new char[ 12 ] };
    Lib::STL::strcpy( w, buf );
    Lib::IO::sout << w << " " << (void *)w << "\n";
    delete[] w;
    auto a = new char[ 12 ];
    Lib::STL::strcpy( a, buf );
    Lib::IO::sout << a << " " << (void *)a << "\n";
    auto b = new char[ 0x200004 ];
    Lib::STL::strcpy( b, buf );
    Lib::IO::sout << b << " " << (void *)b << "\n";
    delete[] b;
    auto c = new char[ 0x12 ];
    Lib::STL::strcpy( c, buf );
    Lib::IO::sout << c << " " << (void *)c << "\n";
    auto d = new char[ 0x123 ];
    Lib::STL::strcpy( d, buf );
    Lib::IO::sout << d << " " << (void *)d << "\n";
    delete[] c;
    auto f = new char[ 0x200004 ];
    Lib::STL::strcpy( f, buf );
    delete[] f;
    Lib::IO::sout << "Test 2 : Make 2 processes-------------------------------\n";
    /* 开启中断 */
    Kernel::TaskManagement::create( ProcC, 20, "Process C", Kernel::TaskManagement::TaskType::PF_KERNEL_PROCESS, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ), 31 );
    Kernel::TaskManagement::create( ProcD, 20, "Process D", Kernel::TaskManagement::TaskType::PF_KERNEL_PROCESS, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ), 31 );
    Kernel::TaskManagement::create( ProcE, 20, "Process E", Kernel::TaskManagement::TaskType::PF_KERNEL_PROCESS, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ), 31 );
    Kernel::TaskManagement::create( ThreadA, 10, Kernel::TaskManagement::TaskType::PF_KERNEL_THREAD, "Thread A", 31, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ) );
    Kernel::TaskManagement::create( ThreadB, 20, Kernel::TaskManagement::TaskType::PF_KERNEL_THREAD, "Thread B", 31, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ) );
    Utils::sti( );
    while ( true ) {
        Kernel::TaskManagement::block( Kernel::TaskManagement::TaskStatus::BLOCKED );
    }

    return Status;
}
