#include <Kernel/task.hpp>
#include <Lib/IO/Stream/iostream>
PUBLIC namespace QuantumNEC::Kernel {
    TaskManagement::TaskManagement( IN Lib::Types::Ptr< Lib::Types::BootConfig > ) :
        ThreadManagement { } {
    }
    TaskManagement::~TaskManagement( VOID ) {
    }
}