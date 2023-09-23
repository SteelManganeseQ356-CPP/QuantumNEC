#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/STL/list>
PUBLIC namespace QuantumNEC::Lib::Base {
    typedef struct
    {
        volatile int value;
        Lib::STL::ListNode waiters;
    } SemaPhore;
    typedef struct
    {
        //task_struct_t *holder;
        SemaPhore semaPhore;
        int holderRepeatNumber;
    } Lock;
}