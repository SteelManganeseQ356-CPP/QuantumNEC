#include <Kernel/Task/process.hpp>
#include <Kernel/Task/thread.hpp>
#include <Arch/Arch.hpp>
#include <Lib/IO/Stream/iostream>
#include <Lib/STL/string>
#include <cstring>
PUBLIC namespace {
    using namespace QuantumNEC::Kernel;

    TaskLock lock { };
}
PUBLIC namespace QuantumNEC::Kernel {
    Thread::Thread( VOID ) noexcept :
        Process { } {
    }
    Thread::~Thread( VOID ) noexcept {
    }
    auto Thread::create( IN TaskFunction entry, IN CONST Lib::Types::uintptr_t arg, IN CONST TaskType type, IN CONST Lib::Types::Ptr< CONST Lib::Types::char_t > name, IN Lib::Types::uint64_t priority, IN Lib::Types::int64_t flags )->Lib::Types::Ptr< ThreadPCB > {
        lock.acquire( );
        if ( type != TaskType::PF_KERNEL_THREAD && type != TaskType::PF_USER_THREAD ) {
            return NULL;
        }
        Lib::Types::Ptr< ThreadPCB > task { reinterpret_cast< decltype( task ) >( PageMemory::malloc( 1, PageMemory::MemoryPageType::PAGE_2M ) ) };
        if ( !task ) {
            return NULL;
        }
        // 添加到队列
        task->all_task_queue.container = task;
        task->general_task_queue.container = task;
        Lib::STL::list_add_to_end( &ready_task_queue, &task->general_task_queue );
        Lib::STL::list_add_to_end( &all_task_queue, &task->all_task_queue );
        // 没有页表所以置为空
        task->page_directory = NULL;
        // memort map因为page_directory为空所以无效
        // 基本上与process分配类似
        task->stack = reinterpret_cast< decltype( task->stack ) >( reinterpret_cast< Lib::Types::uint64_t >( task ) + sizeof( ThreadPCB ) );
        task->message.send_to = NO_TASK;
        task->message.receive_from = NO_TASK;
        Lib::STL::list_init( &task->message.sender_list );
        task->PID = pid_pool.allocate( );
        Lib::STL::strncpy( task->name, name, TASK_NAME_SIZE );
        task->status = TaskStatus::READY;
        task->type = type;
        task->counter = 0;
        task->ticks = priority;
        task->signal = 0;
        task->priority = priority;
        task->flags = flags;
        task->stack_magic = TASK_STACK_MAGIC;
        task->cpu_frame = reinterpret_cast< Lib::Types::Ptr< ThreadFrame > >( reinterpret_cast< Lib::Types::uint64_t >( task ) + TASK_STACK_SIZE - sizeof( ThreadFrame ) );
        if ( auto current_fpu { Process::get_current< ProcessPCB >( )->fpu_frame }; current_fpu ) {
            task->fpu_frame = reinterpret_cast< Lib::Types::Ptr< Architecture::ArchitectureManager< TARGET_ARCH >::FPUFrame > >( reinterpret_cast< Lib::Types::uint64_t >( task->cpu_frame ) - sizeof *current_fpu );
            Lib::STL::memcpy( task->fpu_frame, current_fpu, sizeof *current_fpu );
        }
        Thread::do_fork( task->cpu_frame, entry, arg );
        lock.release( );
        return task;
    }
    auto Thread::do_fork( OUT Lib::Types::Ptr< ThreadFrame > frame, IN TaskFunction entry, IN Lib::Types::uintptr_t arg )->VOID {
#if defined( __x86_64__ )
        // 没啥好解释的。。。
        frame->rdi = arg;
        frame->rsi = 0;
        frame->rip = reinterpret_cast< decltype( frame->rip ) >( entry );
        frame->rbp = 0;
        frame->rbx = 0;
        frame->r15 = 0;
        frame->r14 = 0;
        frame->r13 = 0;
        frame->r12 = 0;
#elif defined( __aarch64__ )
#else
#error There are no instructions available to set the registers
#endif
    }
    auto Thread::block( IN TaskStatus status )->VOID {
        Architecture::ArchitectureManager< TARGET_ARCH >::InterruptStatus interrupt_status { Architecture::ArchitectureManager< TARGET_ARCH >::disable_interrupt( ) };
        Lib::Types::Ptr< ThreadPCB > current { reinterpret_cast< decltype( current ) >( get_current< ThreadPCB >( ) ) };
        if ( current == reinterpret_cast< decltype( current ) >( kernel_task ) ) {
            current = reinterpret_cast< decltype( current ) >( main_task );
        }
        current->status = status;
        Process::schedule( );
        Architecture::ArchitectureManager< TARGET_ARCH >::set_interrupt( interrupt_status );
        return;
    }
    auto Thread::unblock( IN Lib::Types::Ptr< VOID > pcb )->VOID {
        if ( !pcb ) {
            return;
        }
        Lib::Types::Ptr< ThreadPCB > thread_pcb { reinterpret_cast< decltype( thread_pcb ) >( pcb ) };
        if ( thread_pcb == reinterpret_cast< decltype( thread_pcb ) >( kernel_task ) ) {
            thread_pcb = reinterpret_cast< decltype( thread_pcb ) >( main_task );
        }
        Architecture::ArchitectureManager< TARGET_ARCH >::InterruptStatus interrupt_status { Architecture::ArchitectureManager< TARGET_ARCH >::disable_interrupt( ) };
        if ( thread_pcb->status != TaskStatus::READY ) {
            // 放到队列的最前面，使其尽快得到调度
            Lib::STL::list_add_to_head( &ready_task_queue, &thread_pcb->general_task_queue );
            thread_pcb->status = TaskStatus::READY;
        };
        Architecture::ArchitectureManager< TARGET_ARCH >::set_interrupt( interrupt_status );
        return;
    }
    auto TaskLock::release( VOID )->VOID {
        std::atomic_flag_clear_explicit( this, std::memory_order_release );
    }
    auto TaskLock::acquire( VOID )->VOID {
        while ( std::atomic_flag_test_and_set_explicit( this, std::memory_order_acquire ) ) {
            Architecture::ArchitectureManager< TARGET_ARCH >::pause( );
        }
    }
}