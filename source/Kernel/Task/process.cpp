#include <Kernel/Task/process.hpp>
#include <Kernel/Task/thread.hpp>
#include <Lib/STL/string>
#include <Arch/Arch.hpp>
#include <Utils/asm.hpp>
PUBLIC namespace {
    using namespace QuantumNEC;
    using namespace QuantumNEC::Kernel;
    using namespace QuantumNEC::Kernel::Task;
    Task::TaskLock lock { };
    /**
     * @brief 进程起始函数（类似于_start）
     */
    PRIVATE auto process_entry( VOID )->VOID {
        Kernel::Task::ProcessManagement::TaskFunction entry { };
        Kernel::Task::ProcessManagement::TaskArg arg { };
        // 取出存在寄存器中的临时变量
#if defined( __x86_64__ )
        ASM( "MOVQ %%RDI, %0\n\t"
             "MOVQ %%RSI, %1\n\t"
             "MOVQ $0, %%RDI\n\t"
             "MOVQ $0, %%RSI\n\t"
             : "=g"( entry ), "=g"( arg )
             :
             : );
#elif defined( __aarch64__ )
#else
#error There are no instructions available to retrieve registers
#endif

        ArchitectureManagement< TARGET_ARCH >::InterruptManagement::enable_interrupt( );
        auto return_value { entry( arg ) };
        Lib::IO::sout[ Lib::IO::ostream::HeadLevel::SYSTEM ] << "A Task Exited. Return Value is " << return_value << Lib::IO::endl;
        Kernel::Task::ProcessManagement::schedule( );
    }
}
PUBLIC namespace QuantumNEC::Kernel::Task {
    ProcessManagement::ProcessManagement( VOID ) {
    }
    ProcessManagement::~ProcessManagement( VOID ) {
    }
    auto ProcessManagement::create( IN TaskFunction entry, IN CONST TaskArg arg, IN CONST Lib::Types::Ptr< CONST Lib::Types::char_t > name, IN CONST TaskType type, IN CONST Lib::Types::int64_t flags, IN Lib::Types::uint64_t priority )->VOID {
        lock.acquire( );
        Lib::Types::Ptr< ProcessPCB > task { reinterpret_cast< decltype( task ) >( Memory::PageMemoryManagement::malloc( 2, Memory::PageMemoryManagement::MemoryPageType::PAGE_2M ) ) };
        if ( !task ) {
            return;
        }
        // 将任务pcb添加到队列
        task->all_task_queue.container = task;
        task->general_task_queue.container = task;
        Lib::STL::list_add_to_end( &ready_task_queue, &task->general_task_queue );
        Lib::STL::list_add_to_end( &all_task_queue, &task->all_task_queue );

        // 制作页表
        task->page_directory = reinterpret_cast< decltype( task->page_directory ) >( Memory::MemoryMapManagement::make_page_directory_table( ) );
        if ( !task->page_directory ) {
            Lib::STL::memset( task, 0, TASK_STACK_SIZE );
            Memory::PageMemoryManagement::free( task, 2, Memory::PageMemoryManagement::MemoryPageType::PAGE_2M );
            return;
        }
        task->stack = reinterpret_cast< decltype( task->stack ) >( reinterpret_cast< Lib::Types::uint64_t >( task ) + TASK_STACK_SIZE );     // 分配栈空间（2M - 64KB）

        task->allocate_table = Lib::Base::AllocateManagement { sizeof( Lib::Base::AllocateManagement::AllocateTableEntry ) * 1024 };
        task->allocate_table.free( Memory::USER_STACK_VIRTUAL_START_ADDRESS / Memory::PAGE_SIZE, ( Memory::USER_STACK_VIRTUAL_END_ADDRESS - Memory::USER_STACK_VIRTUAL_START_ADDRESS ) / Memory::PAGE_SIZE );

        // 分配 cpu frame
        task->cpu_frame = reinterpret_cast< decltype( task->cpu_frame ) >( reinterpret_cast< Lib::Types::uint64_t >( task->stack ) ) - sizeof( ProcessFrame );
        task->cpu_frame->rsp = reinterpret_cast< decltype( task->cpu_frame->rsp ) >( task->stack );
        task->cpu_frame->rbp = reinterpret_cast< decltype( task->cpu_frame->rbp ) >( task->cpu_frame->rsp );

        if ( !do_fork( task->cpu_frame, entry, arg, type ) ) {
            // 如果分配失败那么就释放所有的内存并返回
            Lib::STL::memset( task->cpu_frame, 0, sizeof( ProcessFrame ) );
            Lib::STL::memset( task, 0, TASK_STACK_SIZE );
            Memory::PageMemoryManagement::free( task, 2, Memory::PageMemoryManagement::MemoryPageType::PAGE_2M );
            return;
        }

        // 分配 fpu frame
        task->fpu_frame = reinterpret_cast< decltype( task->fpu_frame ) >( reinterpret_cast< Lib::Types::uint64_t >( task->cpu_frame ) - sizeof *task->fpu_frame );
        if ( auto current_fpu { ProcessManagement::get_current< ProcessPCB >( )->fpu_frame }; current_fpu ) {
            Lib::STL::memcpy( task->fpu_frame, current_fpu, sizeof *current_fpu );
        }
        // 分配PID
        task->PID = pid_pool.allocate( );
        // 设置进程名
        Lib::STL::strncpy( task->name, name, TASK_NAME_SIZE );
        // 设置进程初始状态
        task->status = TaskStatus::READY;
        // 进程类型
        task->type = type;
        //
        task->counter = 0;
        task->ticks = priority;
        // 暂时没啥用
        task->signal = 0;
        // 时间片越多优先级越高
        task->priority = priority;
        task->flags = flags;
        task->stack_magic = TASK_STACK_MAGIC;
        lock.release( );
    }
    auto ProcessManagement::do_fork( OUT Lib::Types::Ptr< ProcessFrame > frame, IN TaskFunction entry, IN CONST TaskArg arg, IN CONST TaskType type )->Lib::Types::BOOL {
        // 进程执行流程 ：
        // 当前执行进程 -> 跳转进程引导程序 -> 跳转至进程入口（类似与c程序里main函数前要先调用_start） -> 获取参数（arg）与要进入的入口（entry）-> 调用entry -> entry返回后报告并schedule
        frame->rax = 0;
        frame->rbx = 0;
        frame->rcx = 0;
        frame->rdx = 0;
        frame->rbp = 0;
        // rdi存入口, rsi存参数
        frame->rsi = reinterpret_cast< decltype( frame->rsi ) >( arg );
        frame->rdi = reinterpret_cast< decltype( frame->rdi ) >( entry );
        frame->r8 = 0;
        frame->r9 = 0;
        frame->r10 = 0;
        frame->r11 = 0;
        frame->r12 = 0;
        frame->r13 = 0;
        frame->r14 = 0;
        frame->r15 = 0;
        frame->rflags = Architecture::Platform::RFLAGS_IOPL_0 | Architecture::Platform::RFLAGS_MBS | Architecture::Platform::RFLAGS_IF_1;
        // rip存进程入口
        frame->rip = reinterpret_cast< decltype( frame->rip ) >( process_entry );
        if ( type == TaskType::PF_KERNEL_PROCESS ) {
            // 内核进程的
            frame->ds = Architecture::Platform::SELECTOR_DATA64_KERNEL;
            frame->es = Architecture::Platform::SELECTOR_DATA64_KERNEL;
            frame->fs = Architecture::Platform::SELECTOR_DATA64_KERNEL;
            frame->gs = Architecture::Platform::SELECTOR_DATA64_KERNEL;
            frame->cs = Architecture::Platform::SELECTOR_CODE64_KERNEL;
            frame->ss = Architecture::Platform::SELECTOR_DATA64_KERNEL;
        }
        else if ( type == TaskType::PF_USER_PROCESS ) {
            // 用户进程的
            frame->ds = Architecture::Platform::SELECTOR_DATA64_USER;
            frame->es = Architecture::Platform::SELECTOR_DATA64_USER;
            frame->fs = Architecture::Platform::SELECTOR_DATA64_USER;
            frame->gs = Architecture::Platform::SELECTOR_DATA64_USER;
            frame->cs = Architecture::Platform::SELECTOR_CODE64_USER;
            frame->ss = Architecture::Platform::SELECTOR_DATA64_USER;
        }
        else {     // 分配失败
            return FALSE;
        }
        return TRUE;
    }
    auto ProcessManagement::schedule( VOID )->VOID {
        Lib::Types::Ptr< ProcessPCB > current { reinterpret_cast< decltype( current ) >( get_current< ProcessPCB >( ) ) };
        if ( current->all_task_queue.container != current || current->general_task_queue.container != current || current->stack_magic != TASK_STACK_MAGIC ) {
            Lib::IO::sout[ Lib::IO::ostream::HeadLevel::WARNING ] << "Task allocate error." << '\n';
            while ( TRUE )
                ;
        }
        if ( current->status == TaskStatus::RUNNING ) {
            if ( Lib::STL::list_find( &ready_task_queue, &current->general_task_queue ) ) {
                Lib::IO::sout[ Lib::IO::ostream::HeadLevel::WARNING ] << "Can not find the task." << '\n';
                while ( TRUE )
                    ;
            }
            Lib::STL::list_add_to_end( &ready_task_queue, &current->general_task_queue );
            current->status = TaskStatus::RUNNING;
        }
        if ( Lib::STL::list_is_empty( &ready_task_queue ) ) {
            ThreadManagement::reblock( ThreadManagement::idle_thread );
        }
        Lib::Types::Ptr< Lib::STL::ListNode > next_task_queue { Lib::STL::list_pop( &ready_task_queue ) };
        Lib::Types::Ptr< ProcessPCB > next { reinterpret_cast< decltype( next ) >( next_task_queue->container ) };
        next->status = TaskStatus::RUNNING;
        ArchitectureManagement< TARGET_ARCH >::disable_fpu( current );     // 关闭FPU
        activate_task< ProcessPCB >( next );                               // 激活页表
        if ( next->type == TaskType::PF_KERNEL_PROCESS || next->type == TaskType::PF_USER_PROCESS ) {
#if defined( __x86_64__ )
            ASM(
                "MOVQ %0, %%RSP\n\t"
                "JMP process_switch\n\t" : : "g"( next->cpu_frame ) : "memory" );
#elif defined( __aarch64__ )
#else
#error There are no instructions available to retrieve registers
#endif
        }
        else {
            Architecture::ArchitectureManagement< TARGET_ARCH >::asm_switch_to( &current->cpu_frame, &next->cpu_frame );
        }
    }
}
