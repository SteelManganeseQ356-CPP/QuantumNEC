#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Kernel/Task/process.hpp>
#include <Kernel/Task/thread.hpp>
PUBLIC namespace QuantumNEC::Kernel {
    PUBLIC class TaskManagement :
        public Task::ThreadManagement
    {
    public:
        explicit( true ) TaskManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept;
        virtual ~TaskManagement( VOID ) noexcept( true );

    public:
        STATIC auto create( IN TaskFunction entry, IN CONST TaskArg arg, IN CONST TaskType type, IN IN CONST Lib::Types::Ptr< CONST Lib::Types::char_t > name, IN Lib::Types::uint64_t priority, IN CONST Lib::Types::int64_t flags ) -> Lib::Types::Ptr< VOID > {
            if ( type == TaskType::PF_KERNEL_PROCESS || type == TaskType::PF_USER_PROCESS ) {
                return ProcessManagement::create( entry, arg, type, name, priority, flags );
            }
            else if ( type == TaskType::PF_KERNEL_THREAD || type == TaskType::PF_USER_THREAD ) {
                return ThreadManagement::create( entry, arg, type, name, priority, flags );
            }
            return NULL;
        }

    private:
    };
}
