#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
#include <Arch/Arch.hpp>
#include <Kernel/memory.hpp>
#include <Lib/Base/allocate.hpp>
#include <Lib/Base/attribute.hpp>
#include <Lib/STL/list>
#include <Lib/STL/string>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Kernel::Task {
    PUBLIC constexpr CONST auto TASK_STACK_SIZE { 65536 };     // 64KB
    PUBLIC constexpr CONST auto TASK_STACK_MAGIC { 0x1145141919810ULL };
    PUBLIC constexpr CONST auto TASK_NAME_SIZE { 32 };
    PUBLIC constexpr CONST auto PID_COUNT { 1024 };
    PUBLIC class ProcessManagement
    {
    public:
        using TaskFunction = Lib::Types::FuncPtr< Lib::Types::int64_t, Lib::Types::uintptr_t >;
        using TaskArg = Lib::Types::uint64_t;

        enum class TaskType : Lib::Types::uint64_t {
            PF_KERNEL_THREAD = ( 1 << 0 ),
            PF_USER_THREAD = ( 1 << 1 ),
            PF_KERNEL_PROCESS = ( 1 << 2 ),
            PF_USER_PROCESS = ( 1 << 3 ),
        };

        enum class TaskStatus : Lib::Types::int64_t {
            RUNNING,
            READY,
            BLOCKED,
            SENDING,
            RECEIVING,
            WAITING,
            HANGING,
            DIED,
        };
        enum class TaskFlags : Lib::Types::uint64_t {
            FPU_UNUSED = 0,
            FPU_USED = 1,
            FPU_ENABLED = 2,
        };

    public:
        /**
         * @brief 程序控制块PCB
         * @tparam Frame PCB属于进程还是线程
         */
        template < typename Frame >
        struct TaskPCB
        {
            /*
                内部是像这样的 :
                栈底 ~ 栈顶 = 64KB
                栈顶往前一个ProcessFrame大小就是处理机状态信息保存区，
                处理机状态信息保存区往前就是FPU状态信息保存区
                栈底 ~ FPU状态信息保存区开头这一片区域都是进程控制信息
            */
            // 连接每个任务
            Lib::STL::ListNode all_task_queue;         // 所有任务队列
            Lib::STL::ListNode general_task_queue;     // 通用任务队列
            // 记录内存分布
            Lib::Types::Ptr< Lib::Types::uint64_t > page_directory;     // 任务所持有的页表地址
            Lib::Types::Ptr< VOID > stack;                              // 任务所持有的栈起始地址
            Lib::Base::AllocateManagement allocate_table;               // 虚拟地址表，在page_directory不为空时有效
            // 记录寄存器状态
            Lib::Types::Ptr< Frame > cpu_frame;                                               // 保存的CPU寄存器状态信息
            Lib::Types::Ptr< ArchitectureManagement< TARGET_ARCH >::FPUFrame > fpu_frame;     // 保存的FPU寄存器状态信息
            // 任务信息
            Lib::Types::int64_t PID;                       // 任务ID
            Lib::Types::char_t name[ TASK_NAME_SIZE ];     // 任务名
            volatile TaskStatus status;                    // 任务状态
            TaskType type;                                 // 任务类型
            Lib::Types::int64_t counter;                   // 任务总时间片
            Lib::Types::int64_t ticks;                     // 每次在处理器上执行的时间嘀嗒数
            Lib::Types::int64_t signal;                    // 任务持有的信号
            Lib::Types::int64_t priority;                  // 任务优先级
            Lib::Types::int64_t flags;                     // 任务特殊标志
            Lib::Types::uint64_t stack_magic;              // 用于检测栈的溢出

            explicit( TRUE ) TaskPCB( VOID ) noexcept = default;
            explicit( TRUE ) TaskPCB( IN Lib::Types::L_Ref< CONST TaskPCB< Frame > > object ) noexcept
                :
                all_task_queue { object.all_task_queue },
                general_task_queue { object.general_task_queue },
                page_directory { object.page_directory },
                stack { object.stack },
                allocate_table { object.allocate_table },
                cpu_frame { object.cpu_frame },
                fpu_frame { object.fpu_frame },
                PID { object.PID },
                name { },
                status { object.status },
                type { object.type },
                counter { object.counter },
                ticks { object.ticks },
                signal { object.signal },
                priority { object.priority },
                flags { object.flags },
                stack_magic { object.stack_magic } {
                Lib::STL::strncpy( this->name, object.name, TASK_NAME_SIZE );
            };
            auto operator=( IN CONST Lib::Types::L_Ref< TaskPCB< Frame > > object ) -> Lib::Types::L_Ref< TaskPCB< Frame > > {
                this->all_task_queue = object.all_task_queue;
                this->general_task_queue = object.general_task_queue;
                this->page_directory = object.page_directory;
                this->stack = object.stack;
                this->allocate_table = object.allocate_table;
                this->cpu_frame = object.cpu_frame;
                this->fpu_frame = object.fpu_frame;
                this->PID = object.PID;
                this->status = object.status;
                this->type = object.type;
                this->counter = object.counter;
                this->ticks = object.ticks;
                this->signal = object.signal;
                this->priority = object.priority;
                this->flags = object.flags;
                this->stack_magic = object.stack_magic;
                Lib::STL::strncpy( this->name, object.name, TASK_NAME_SIZE );
                return *this;
            }
        };

    public:
        typedef struct :
#if defined( __x86_64__ )
            Architecture::Platform::SegmentRegisterFrame,
            Architecture::Platform::GeneralRegisterFrame
#elif defined( __aarch64__ )
#else
#error Not any registers
#endif
        {
#if defined( __x86_64__ )
            Lib::Types::uint64_t reserved[ 2 ];
            Lib::Types::Ptr< VOID > rip;
            Lib::Types::uint64_t cs;
            Lib::Types::uint64_t rflags;
            Lib::Types::uint64_t rsp;
            Lib::Types::uint64_t ss;
#elif defined( __aarch64__ )
#else
#error Not any registers
#endif
        } ProcessFrame;     // 进程栈

        using ProcessPCB = TaskPCB< ProcessFrame >;

    public:
        class PidPool
        {
        public:
            auto allocate( VOID ) -> Lib::Types::uint64_t {
                this->bitmap_.set_length( PID_COUNT );
                this->bitmap_.set_bits( this->bits );
                auto pid_index { this->bitmap_.allocate( 1 ) };
                this->bitmap_.set( pid_index, 1 );
                return pid_index + this->pid_head;
            }

        private:
            inline STATIC Lib::Types::uint64_t pid_head { };
            inline STATIC Lib::Types::byte_t bits[ PID_COUNT ] { };
            inline STATIC Lib::Base::BitmapManagement bitmap_ { };
        };
        inline STATIC PidPool pid_pool { };

    public:
        explicit( TRUE ) ProcessManagement( ) noexcept( TRUE );
        virtual ~ProcessManagement( VOID ) noexcept( TRUE );

    public:
        /**
         * @brief 调度
         */
        STATIC auto schedule( VOID ) -> VOID;
        /**
         * @brief 获取当前进程/线程pcb
         * @tparam PCB 要获取的PCB类型
         * @return 获取到的PCB
         */
        template < typename PCB >
        STATIC auto get_current( VOID ) -> Lib::Types::Ptr< PCB > {
            return reinterpret_cast< Lib::Types::Ptr< PCB > >( Utils::get_rsp( ) & ~( TASK_STACK_SIZE - 1 ) );
        }
        /**
         * @brief 激活进程/线程
         * @tparam PCB 要激活的PCB类型
         * @param pcb 要激活的进程/线程PCB
         */
        template < typename PCB >
        STATIC auto activate_task( IN Lib::Types::Ptr< PCB > pcb ) -> VOID {
            Memory::MemoryMapManagement::activate_page_directory_table( pcb->page_directory );
            Architecture::CPU::GlobalSegmentDescriptorManagement::set_tss_rsp0( 0, pcb, TASK_STACK_SIZE );
        }

    public:
        /**
         * @brief 创建任务
         * @param entry 任务入口函数
         * @param name 任务名
         * @param type 任务PCB的类型
         */
        STATIC auto create( IN TaskFunction entry, IN CONST TaskArg arg, IN IN CONST Lib::Types::Ptr< CONST Lib::Types::char_t > name, IN CONST TaskType type, IN CONST Lib::Types::int64_t flags, IN Lib::Types::uint64_t priority ) -> VOID;
        /**
         * @brief 初始化进程栈
         * @param frame 分配好栈的地址
         * @param entry 线程入口函数
         * @param arg 要传递给entry的参数
         */
        STATIC auto do_fork( OUT Lib::Types::Ptr< ProcessFrame > frame, IN TaskFunction entry, IN CONST TaskArg arg, IN CONST TaskType type ) -> Lib::Types::BOOL;

    protected:
        inline STATIC Lib::STL::ListTable all_task_queue { };
        inline STATIC Lib::STL::ListTable ready_task_queue { };
    };
}