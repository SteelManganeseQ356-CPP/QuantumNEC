#include <Kernel/Task/thread.hpp>
#include <Lib/IO/Stream/iostream>
#include <Lib/STL/string>
#include <Utils/asm.hpp>
PUBLIC namespace {
    using namespace QuantumNEC;
    PRIVATE auto thread_entry( VOID )->VOID {
        Kernel::Task::ProcessManagement::TaskFunction entry { };
        Kernel::Task::ProcessManagement::TaskArg arg { };
#if defined( __x86_64__ )
        ASM( "MOVQ %%RDI, %0\n\t"
             "MOVQ %%RSI, %1\n\t"
             : "=g"( arg ), "=g"( entry )
             :
             : );
#elif defined( __aarch64__ )
#else
#error There are no instructions available to retrieve registers
#endif

        ArchitectureManagement< TARGET_ARCH >::InterruptManagement::enable_interrupt( );
        auto return_value { entry( arg ) };
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ] << "A Task Exited. Return Value is " << return_value << Lib::IO::endl;
        while ( TRUE ) {
            Kernel::Task::ProcessManagement::schedule( );
        }
    }
    PRIVATE auto idle( IN Kernel::Task::ProcessManagement::TaskArg )->Lib::Types::int64_t {
        while ( TRUE ) {
            Kernel::Task::ThreadManagement::block( Kernel::Task::ThreadManagement::TaskStatus::BLOCKED );
            Utils::sti( );
            Utils::hlt( );
        }
        return 0;
    }
}
PUBLIC namespace QuantumNEC::Kernel::Task {
    STATIC TaskLock lock { };
    ThreadManagement::ThreadManagement( VOID ) :
        ProcessManagement { } {
        // 准备内核进程PCB
        Lib::STL::list_init( &this->all_task_queue );
        Lib::STL::list_init( &this->ready_task_queue );
        this->main_thread = get_current< ThreadPCB >( );
        this->main_thread->all_task_queue.container = this->main_thread;
        this->main_thread->general_task_queue.container = this->main_thread;
        Lib::STL::list_add_to_end( &this->all_task_queue, &this->main_thread->all_task_queue );
        this->main_thread->page_directory = NULL;
        this->main_thread->stack = reinterpret_cast< decltype( this->main_thread->stack ) >( reinterpret_cast< Lib::Types::uint64_t >( get_current< ThreadPCB >( ) ) + TASK_STACK_SIZE );
        this->main_thread->cpu_frame = reinterpret_cast< decltype( this->main_thread->cpu_frame ) >( reinterpret_cast< Lib::Types::uint64_t >( this->main_thread->stack ) - sizeof *this->main_thread->cpu_frame );
        this->main_thread->fpu_frame = reinterpret_cast< decltype( this->main_thread->fpu_frame ) >( reinterpret_cast< Lib::Types::uint64_t >( this->main_thread->cpu_frame ) - sizeof *this->main_thread->fpu_frame );
        this->main_thread->PID = this->pid_pool.allocate( );
        Lib::STL::strcpy( this->main_thread->name, "Kernel Thread" );
        this->main_thread->status = TaskStatus::RUNNING;
        this->main_thread->type = TaskType::PF_KERNEL_THREAD;
        this->main_thread->flags = static_cast< Lib::Types::int64_t >( TaskFlags::FPU_UNUSED );
        this->main_thread->counter = 0;
        this->main_thread->ticks = 31;
        this->main_thread->signal = 0;
        this->main_thread->priority = 0;
        this->main_thread->stack_magic = TASK_STACK_MAGIC;
        this->idle_thread = this->create( idle, 0, TaskType::PF_KERNEL_THREAD, "idle", 31, static_cast< Lib::Types::int64_t >( TaskFlags::FPU_UNUSED ) );
    }
    ThreadManagement::~ThreadManagement( VOID ) {
    }
    auto ThreadManagement::create( IN TaskFunction entry, IN CONST Lib::Types::uintptr_t arg, IN CONST TaskType type, IN CONST Lib::Types::Ptr< CONST Lib::Types::char_t > name, IN Lib::Types::uint64_t priority, IN Lib::Types::int64_t flags )->Lib::Types::Ptr< ThreadPCB > {
        lock.acquire( );
        if ( type != TaskType::PF_KERNEL_THREAD && type != TaskType::PF_USER_THREAD ) {
            return NULL;
        }
        Lib::Types::Ptr< ThreadPCB > task { reinterpret_cast< decltype( task ) >( Memory::PageMemoryManagement::malloc( 1, Memory::PageMemoryManagement::MemoryPageType::PAGE_2M ) ) };
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
        task->stack = reinterpret_cast< decltype( task->stack ) >( reinterpret_cast< Lib::Types::uint64_t >( task ) + TASK_STACK_SIZE );
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
        task->cpu_frame = reinterpret_cast< decltype( task->cpu_frame ) >( reinterpret_cast< Lib::Types::uint64_t >( task->stack ) - sizeof( ThreadFrame ) );
        if ( auto current_fpu { ProcessManagement::get_current< ProcessPCB >( )->fpu_frame }; current_fpu ) {
            task->fpu_frame = reinterpret_cast< decltype( task->fpu_frame ) >( reinterpret_cast< Lib::Types::uint64_t >( task->cpu_frame ) - sizeof *current_fpu );
            Lib::STL::memcpy( task->fpu_frame, current_fpu, sizeof *current_fpu );
        }
        ThreadManagement::do_fork( task->cpu_frame, entry, arg );
        lock.release( );
        return task;
    }
    auto ThreadManagement::do_fork( OUT Lib::Types::Ptr< ThreadFrame > frame, IN TaskFunction entry, IN Lib::Types::uintptr_t arg )->VOID {
#if defined( __x86_64__ )
        // 没啥好解释的。。。
        frame->rdi = arg;
        frame->rsi = reinterpret_cast< decltype( frame->rsi ) >( entry );
        frame->rip = reinterpret_cast< decltype( frame->rip ) >( thread_entry );
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
    auto ThreadManagement::block( IN TaskStatus status )->VOID {
        if ( !( status != TaskStatus::RUNNING && status != TaskStatus::READY ) ) {
            while ( TRUE )
                ;
        }

        Architecture::ArchitectureManagement< TARGET_ARCH >::InterruptStatus interrupt_status { Architecture::ArchitectureManagement< TARGET_ARCH >::disable_interrupt( ) };
        Lib::Types::Ptr< ThreadPCB > current { reinterpret_cast< decltype( current ) >( get_current< ThreadPCB >( ) ) };
        current->status = status;
        if ( current->status != status ) {
            while ( TRUE )
                ;
        }
        ProcessManagement::schedule( );
        // 待当前线程被解除阻塞后才继续运行下面的intr_set_status
        Architecture::ArchitectureManagement< TARGET_ARCH >::set_interrupt( interrupt_status );
        return;
    }
    auto ThreadManagement::reblock( IN Lib::Types::Ptr< VOID > pcb )->VOID {
        Lib::Types::Ptr< ThreadPCB > thread_pcb { reinterpret_cast< decltype( thread_pcb ) >( pcb ) };
        Architecture::ArchitectureManagement< TARGET_ARCH >::InterruptStatus interrupt_status { Architecture::ArchitectureManagement< TARGET_ARCH >::disable_interrupt( ) };
        if ( !Lib::STL::list_find( &all_task_queue, &thread_pcb->all_task_queue ) ) {
            while ( TRUE )
                ;
        }
        if ( thread_pcb->status != TaskStatus::READY ) {
            if ( Lib::STL::list_find( &ready_task_queue, &thread_pcb->general_task_queue ) ) {
                while ( TRUE )
                    ;
            }
            // 放到队列的最前面，使其尽快得到调度
            Lib::STL::list_add_to_head( &ready_task_queue, &thread_pcb->general_task_queue );

            thread_pcb->status = TaskStatus::READY;
        };
        Architecture::ArchitectureManagement< TARGET_ARCH >::set_interrupt( interrupt_status );
        return;
    }
    auto TaskLock::release( VOID )->VOID {
        std::atomic_flag_clear_explicit( this, std::memory_order_release );
    }
    auto TaskLock::acquire( VOID )->VOID {
        while ( std::atomic_flag_test_and_set_explicit( this, std::memory_order_acquire ) ) {
            Utils::pause( );
        }
    }
}