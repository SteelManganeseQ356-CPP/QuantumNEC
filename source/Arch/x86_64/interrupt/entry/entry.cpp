#include <Arch/x86_64/platform/platform.hpp>
PUBLIC namespace QuantumNEC::Architecture::Interrupt::InterruptEntry {
    InterruptEntryRegister::InterruptEntryRegister( VOID ) :
        TimeEntry { } {
        // 注册时钟中断入口函数
    }
    InterruptEntryRegister::~InterruptEntryRegister( VOID ) {
    }
}