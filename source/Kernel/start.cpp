#include <Arch/Arch.hpp>
#include <Kernel/kernel.hpp>
#include <Kernel/memory.hpp>
#include <Kernel/task.hpp>
#include <Lib/IO/Stream/iostream>
using namespace QuantumNEC;
using namespace QuantumNEC::Lib;
using namespace QuantumNEC::Lib::Types;

//////////////////////////////////////分割线///////////////////////////////////////////////
/////////////////////////////////////以下为测试/////////////////////////////////////////////
Lib::Types::int64_t ThreadA( Lib::Types::uint64_t code ) {
    // Kernel::Task::MessageManagement message { };
    // message->se

    Lib::IO::sout << "Thread A : " << code << '\n';
    Lib::IO::sout << "PID:" << Kernel::Task::get_current< Kernel::Task::ThreadPCB >( )->PID << '\n';

    while ( true )
        ;

    return 0;
}
Lib::Types::int64_t ThreadB( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Thread B : " << code << '\n';
    Lib::IO::sout << "PID:" << Kernel::Task::get_current< Kernel::Task::ThreadPCB >( )->PID << '\n';
    while ( true )
        ;

    return 0;
}
Lib::Types::int64_t ThreadC( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Thread C : " << code << '\n';
    Lib::IO::sout << "PID:" << Kernel::Task::get_current< Kernel::Task::ThreadPCB >( )->PID << '\n';

    while ( true )
        ;

    return 0;
}
Lib::Types::int64_t ProcC( Lib::Types::uint64_t ) {
    // int i { };
    // while ( true ) {
    //     Lib::IO::sout << "                          C:             " << ++i << '\n';
    // }
    // Lib::IO::sout << "Process C: " << code << "\n";

    // Lib::IO::sout << Kernel::TaskManagement::get_current< Kernel::TaskManagement::ThreadPCB >( )->PID << '\n';

    // //  message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_RECEIVE, 3 );
    // message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_SEND_RECEIVE, 3 );
    // Lib::IO::sout << "C2:\n";
    // message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_SEND_RECEIVE, 3 );

    // message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_SEND, 2 );
    // message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_SEND_RECEIVE, 2 );
    // Lib::IO::sout << "Process C: " << code << "\n"
    //               << Kernel::TaskManagement::get_current< Kernel::TaskManagement::ProcessPCB >( )->PID << "\n";

    Kernel::Message message { };
    while ( true ) {
        message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_RECEIVE, Kernel::ANY );
        Lib::IO::sout << "C:" << 3 << "\n";
        message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_SEND, 3 );
    }
    while ( true )
        ;
    return 0;
}
Lib::Types::int64_t ProcD( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Process D: " << code << "\n";
    Kernel::Message message { };
    message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_SEND, 2 );
    Lib::IO::sout << "D\n";
    message.send_receive( Architecture::ArchitectureManager< TARGET_ARCH >::SyscallFunction::MESSAGE_SEND, 2 );

    while ( true )
        ;

    return 0;
}

Lib::Types::int64_t ProcE( Lib::Types::uint64_t code ) {
    // int i { };
    // while ( true ) {
    //     Lib::IO::sout << "E:" << ++i << '\n';

    // }
    Lib::IO::sout << "Process E: " << code << "\n";
    // int i = 0;
    // while ( true ) {
    //     Lib::IO::sout << "E: " << ++i << "\n";
    // }
    while ( true )
        ;
    return 0;
}
Lib::Types::int64_t ProcF( Lib::Types::uint64_t code ) {
    Lib::IO::sout << "Process F: " << code << "\n";
    // int i { };
    // while ( true ) {
    //     Lib::IO::sout << "F:" << ++i << '\n';
    // }
    while ( true )
        ;
    return 0;
}
//////////////////////////////////////分割线///////////////////////////////////////////////

_C_LINK auto micro_kernel_entry( IN Ptr< BootConfig > config ) -> SystemStatus {
    SystemStatus Status { SYSTEM_SUCCESS };
    Architecture::ArchitectureManager< TARGET_ARCH > architecture { config };     // 系统架构初始化
    Kernel::Memory memory { config };                                             // 内存管理初始化
    Kernel::Task task { config };                                                 // 进程管理初始化
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
    Lib::IO::sout << f << " " << (void *)f << "\n";
    delete[] f;
    Lib::IO::sout << "Test 2 : Make 2 processes-------------------------------\n";
    /* 开启中断 */
    //__asm__ __volatile__( "int $0x80\n\t" );

    task.create( ProcC, 20, Kernel::Task::TaskType::PF_KERNEL_PROCESS, "Process C", 31, static_cast< Lib::Types::int64_t >( Kernel::Task::TaskFlags::FPU_UNUSED ) );
    task.create( ProcD, 20, Kernel::Task::TaskType::PF_KERNEL_PROCESS, "Process D", 31, static_cast< Lib::Types::int64_t >( Kernel::Task::TaskFlags::FPU_UNUSED ) );
    // Kernel::Task::create( ProcE, 20, Kernel::Task::TaskType::PF_KERNEL_PROCESS, "Process E", 100, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ) );
    //   Kernel::Task::create( ThreadA, 20, Kernel::Task::TaskType::PF_KERNEL_THREAD, "Thread A", 1000, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ) );
    //   Kernel::Task::create( ThreadB, 20, Kernel::Task::TaskType::PF_KERNEL_THREAD, "Thread B", 31, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ) );
    //  Kernel::Task::create( ProcF, 20, Kernel::Task::TaskType::PF_KERNEL_PROCESS, "Process F", 100, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ) );
    //    // Kernel::Task::create( ThreadC, 20, Kernel::Task::TaskType::PF_KERNEL_THREAD, "Thread C", 100, static_cast< Lib::Types::int64_t >( Kernel::TaskManagement::TaskFlags::FPU_UNUSED ) );
    //    // Utils::sti( );
    task.block( Kernel::Task::TaskStatus::BLOCKED );
    ASM( "sti\n\t" );

    while ( true )
        ;

    return Status;
}
