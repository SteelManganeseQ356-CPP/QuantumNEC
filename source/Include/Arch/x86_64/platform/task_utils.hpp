#ifndef _PLATFORM_HPP_
#error Do not include the header file. Please include platform.hpp
#endif
#pragma once
#include <Lib/Base/deflib.hpp>
#include <Lib/Types/type_ptr.hpp>
#include <Utils/asm.hpp>
/*
 * 本文件仅仅用于task.cpp
 */

PUBLIC namespace QuantumNEC::Architecturse::Platform {
    auto asm_switch_to( IN OUT Lib::Types::Ptr< VOID > current, IN Lib::Types::Ptr< VOID > next )->VOID {
        __asm__ __volatile__(
            "PUSHQ %1\n\t"
            "PUSHQ %0\n\t"
            "LEAQ _asm_switch_to(%%RIP), %%RAX\n\t"
            "CALLQ *%%RAX\n\t"
            :
            : "g"( current ),
              "g"( next )
            : );
    };
}