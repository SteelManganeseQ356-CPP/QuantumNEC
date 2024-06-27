#pragma once

#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
#include <Kernel/Task/process.hpp>
#include <Lib/Debug/assert.hpp>
#include <atomic>
PUBLIC namespace QuantumNEC::Kernel {
    PUBLIC class Thread : public Process     // 任务管理
    {
    public:
        typedef struct ThreadFrame     // 栈底
        {
#if defined( __x86_64__ )
            // ABI要求保护的寄存器
            Lib::Types::uint64_t r15;
            Lib::Types::uint64_t r14;
            Lib::Types::uint64_t r13;
            Lib::Types::uint64_t r12;
            Lib::Types::uint64_t rbx;
            Lib::Types::uint64_t rdi;
            Lib::Types::uint64_t rsi;
            Lib::Types::uint64_t rbp;
            Lib::Types::Ptr< VOID > rip;
            // 栈顶
#elif defined( __aarch64__ )
#else
#error Not any registers
#endif
            explicit ThreadFrame( IN Lib::Types::L_Ref< CONST ThreadFrame > object ) noexcept :
#if defined( __x86_64__ )
                r15 { object.r15 },
                r14 { object.r14 },
                r13 { object.r13 },
                r12 { object.r12 },
                rbx { object.rbx },
                rdi { object.rdi },
                rsi { object.rsi },
                rbp { object.rbp },
                rip { object.rip }

#elif defined( __aarch64__ )

#else
#error Not any registers
#endif
            {
            }
            auto operator=( IN Lib::Types::L_Ref< CONST ThreadFrame > object ) noexcept -> Lib::Types::L_Ref< ThreadFrame > {
#if defined( __x86_64__ )
                r15 = object.r15;
                r14 = object.r14;
                r13 = object.r13;
                r12 = object.r12;
                rbx = object.rbx;
                rdi = object.rdi;
                rsi = object.rsi;
                rbp = object.rbp;
                rip = object.rip;
#elif defined( __aarch64__ )
#else
#error Not any registers
#endif
                return *this;
            }
            // 栈顶
        } _packed ThreadFrame;

        using ThreadPCB = TaskPCB< ThreadFrame >;

    public:
        explicit Thread( VOID ) noexcept;
        virtual ~Thread( VOID ) noexcept;

    public:
        /**
         * @brief 创建线程
         * @param entry 线程入口函数
         * @param arg 要传递给entry的参数
         * @param type 任务PCB的类型
         * @param name 线程名
         * @param priority 任务优先级
         */

        STATIC auto create( IN TaskFunction entry, IN CONST TaskArg arg, IN CONST TaskType type, IN CONST Lib::Types::Ptr< CONST Lib::Types::char_t > name, IN Lib::Types::uint64_t priority, IN CONST Lib::Types::int64_t flags ) -> Lib::Types::Ptr< ThreadPCB >;
        /**
         * @brief 初始化线程栈
         * @param frame 分配好栈的地址
         * @param entry 线程入口函数
         * @param arg 要传递给entry的参数
         */
        STATIC auto do_fork( OUT Lib::Types::Ptr< ThreadFrame > frame, IN TaskFunction entry, IN CONST Lib::Types::uintptr_t arg ) -> VOID;

        /**
         * @brief 阻塞任务
         */
        STATIC auto block( IN TaskStatus status ) -> VOID;
        /**
         * @brief 解除任务阻塞
         * @param pcb 指向的任务PCB
         */
        STATIC auto unblock( IN Lib::Types::Ptr< VOID > pcb ) -> VOID;
    };
    class TaskLock : std::atomic_flag
    {
    public:
        explicit( TRUE ) TaskLock( VOID ) noexcept( TRUE ) {
        }
        explicit( TRUE ) TaskLock( IN Lib::Types::BOOL locked ) noexcept( TRUE ) {
            _M_i = locked;
        }
        virtual ~TaskLock( VOID ) noexcept( TRUE ) = default;

    public:
        /**
         * @brief 释放锁
         */
        auto release( VOID ) -> VOID;
        /**
         * @brief 获取锁
         */
        auto acquire( VOID ) -> VOID;
        bool locked( ) {
            return this->_M_i;
        }
    };
}