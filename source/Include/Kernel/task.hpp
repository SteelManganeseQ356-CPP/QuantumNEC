#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Kernel/Task/process.hpp>
#include <Kernel/Task/thread.hpp>
PUBLIC namespace QuantumNEC::Kernel {
    PUBLIC class TaskManagement :
        public Task::ThreadManagement
    {
    public:
        explicit( true ) TaskManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > _config ) noexcept( true );
        virtual ~TaskManagement( VOID ) noexcept( true );

    public:
    private:
    };
}
