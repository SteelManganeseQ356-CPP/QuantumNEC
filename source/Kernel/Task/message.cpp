#include <Kernel/Task/message.hpp>
#include <Kernel/Task/process.hpp>
#include <Kernel/Task/thread.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Kernel {
    auto Message::send_receive( IN Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallFunction function, IN Lib::Types::uint64_t source_destination )->Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus {
        Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus status { };
        using enum Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallFunction;
        using enum Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus;
        switch ( function ) {
        case MESSAGE_SEND:
        case MESSAGE_RECEIVE:
            // RBX放系统调用的功能号，RVX放源进程或者要被服务进程，RDX就是message，并用RAX接收返回值
            ASM( "INT $0x80\n\t" : "=a"( status ) : "b"( function ), "c"( source_destination ), "d"( this ) );
            break;
        case MESSAGE_SEND_RECEIVE:
            ASM( "INT $0x80\n\t" : "=a"( status ) : "b"( MESSAGE_SEND ), "c"( source_destination ), "d"( this ) );
            if ( status == SUCCESS ) {
                ASM( "INT $0x80\n\t" : "=a"( status ) : "b"( MESSAGE_RECEIVE ), "c"( source_destination ), "d"( this ) );
            }
            break;
        default:
            break;
        }
        return status;
    }

    auto Message::send( IN Lib::Types::uint64_t destination )->Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus {
        Lib::Types::Ptr< Process::ProcessPCB > sender { Process::get_current< Process::ProcessPCB >( ) }, task_destination { };
        sender->message.send_to = destination;
        task_destination = Process::find< Process::ProcessPCB >( destination );
        if ( !task_destination ) {     // 找不到就说明是错的
            sender->message.send_to = NO_TASK;
            return Architecture::ArchitectureManager< TARGET_ARCH >::Syscall ::SyscallStatus::NO_DESTINATION;
        }
        this->source = sender->PID;

        // 判断是否死锁
        if ( auto is_deadlock = [ & ] -> Lib::Types::BOOL {
                 if ( sender->PID == static_cast< decltype( sender->PID ) >( destination ) ) {
                     return TRUE;
                 }
                 Lib::Types::Ptr< Process::ProcessPCB > task { Process::find< Process::ProcessPCB >( destination ) };
                 for ( ;; ) {
                     if ( task->status == Process::TaskStatus::SENDING ) {
                         if ( static_cast< decltype( sender->PID ) >( task->message.send_to ) == sender->PID ) {
                             return TRUE;     // 是死锁了
                         }
                         task = Process::find< Process::ProcessPCB >( task->message.send_to );
                         if ( !task ) {
                             return FALSE;
                         }
                     }
                     else {
                         break;
                     }
                 }
                 return FALSE;
             };
             is_deadlock( ) ) {
            // 死锁就滚
            return Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus::DEADLOCK;
        }
        // 消息复制到当前进程pcb
        Lib::STL::memcpy( &sender->message, this, sizeof *this );

        // 加入队列
        Lib::STL::list_add_to_end( &task_destination->message.sender_list, &sender->general_task_queue );
        // 对方正准备接收消息 ，并且接收消息来自任何地方或接收这个任务的消息
        if ( ( task_destination->status == Process::TaskStatus::RECEIVING ) && ( task_destination->message.receive_from == ANY || static_cast< decltype( sender->PID ) >( task_destination->message.receive_from ) == sender->PID ) ) {
            // 解除对面阻塞
            Thread::unblock( task_destination );
        }

        // 阻塞自己，切换到下一任务
        Thread::block( Process::TaskStatus::SENDING );

        sender->message.send_to = NO_TASK;
        return Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus::SUCCESS;
    }

    auto Message::receive( IN Lib::Types::uint64_t source )->Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus {
        Lib::Types::Ptr< Process::ProcessPCB > receiver { Process::get_current< Process::ProcessPCB >( ) };     //, task_source { };
        if ( static_cast< decltype( receiver->PID ) >( source ) == receiver->PID ) {
            // 和send同样的道理，死锁马上滚
            return Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus::DEADLOCK;
        }
        receiver->message.receive_from = source;
        // 从任意进程接收消息
        if ( source == ANY || source == INTERRUPT ) {
            // 没有消息，阻塞
            if ( Lib::STL::list_is_empty( &receiver->message.sender_list ) ) {
                Thread::block( Process::TaskStatus::RECEIVING );
            }
            if ( receiver->message.has_int_message ) {
                // 消息来自中断
                this->source = INTERRUPT;
                this->type = INTERRUPT;
                receiver->message.has_int_message = 0;
                return Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus::SUCCESS;
            }
            // 切换到下一任务
        }
        else {
            auto find_pcb { Process::find< Process::ProcessPCB >( source ) };
            if ( !find_pcb ) {
                // 压根就没有这东西，滚
                receiver->message.receive_from = NO_TASK;
                return Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus::NO_SOURCE;
            }
            // 如果对方之前给这个任务发送消息，那么这个任务阻塞
            while ( !Lib::STL::list_find( &receiver->message.sender_list, &find_pcb->general_task_queue ) ) {
                Thread::block( Process::TaskStatus::RECEIVING );
            }
            // task_source = find_pcb;
        }
        // else {
        //     // 如果不是前者，那么则是其它进程的消息
        //     auto find_pcb { ProcessManagement::find< ProcessManagement::ProcessPCB >( source ) };
        //     if ( !find_pcb ) {
        //         // 压根就没有这东西，滚
        //         receiver->message.receive_from = NO_TASK;
        //         return Architecture::ArchitectureManagement< TARGET_ARCH >::SyscallManagement::SyscallStatus::NO_SOURCE;
        //     }
        //     /* 阻塞,直到收到消息 */
        //     while ( !Lib::STL::list_find( &receiver->message.sender_list, &find_pcb->general_task_queue ) ) {
        //         ThreadManagement::block( ProcessManagement::TaskStatus::RECEIVING );

        //     }
        //     Lib::STL::list_delete( &find_pcb->general_task_queue );
        //     task_source = find_pcb;
        // }

        // task_source = reinterpret_cast< decltype( task_source ) >( Lib::STL::list_pop( &receiver->message.sender_list )->container );

        // if ( !task_source ) {
        //     receiver->message.receive_from = NO_TASK;
        //     return Architecture::ArchitectureManagement< TARGET_ARCH >::SyscallManagement::SyscallStatus::NO_SOURCE;
        // }
        // Lib::STL::memcpy( this, &task_source->message, sizeof *this );
        // task_source->message.send_to = NO_TASK;
        // if ( task_source->status == ProcessManagement::TaskStatus::SENDING ) {
        //     ThreadManagement::unblock( task_source );
        // }

        // receiver->ticks += task_source->ticks;
        // task_source->ticks = 0;
        // receiver->message.receive_from = NO_TASK;
        return Architecture::ArchitectureManager< TARGET_ARCH >::Syscall::SyscallStatus::SUCCESS;
    }
}
