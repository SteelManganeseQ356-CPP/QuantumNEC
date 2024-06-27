#include <Kernel/task.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Kernel {
    Task::Task( IN Lib::Types::Ptr< Lib::Types::BootConfig > ) noexcept :
        Thread { } {
    }
    Task::~Task( VOID ) noexcept {
    }
}