#pragma once
#include <Boot/Setup.hpp>
#include <Uefi.h>
#ifdef DEBUG
#define ERROR_RETURNS( Expression )       \
    do                                    \
    {                                     \
        EFI_STATUS Status { Expression }; \
        if ( EFI_ERROR( Status ) )        \
        {                                 \
            return Status;                \
        }                                 \
    } while ( FALSE );
#else
#define ERR_RETS( Expression )
#endif
#ifdef DEBUG
#define ERROR_RETURN( Status )     \
    {                              \
        if ( EFI_ERROR( Status ) ) \
        {                          \
            return;                \
        }                          \
    }
#else
#define ERR_RET( Status )
#endif
/* Get power number of unsigned types X^Y*/
using size_t = unsigned long;
namespace QuantumNEC::Boot {
bool PowerU64( UINT64 X, UINTN Y );
}     // namespace QuantumNEC::Boot
VOID *operator new( size_t size ) noexcept;
VOID *operator new[]( size_t cb ) noexcept;
VOID *operator new( unsigned long int, VOID *address ) noexcept;
VOID operator delete( VOID *, size_t ) noexcept;
VOID operator delete( VOID *address ) noexcept;
VOID operator delete[]( VOID *address ) noexcept;

#define _cpuid( __func, __eax, __ebx, __ecx, __edx )                                  \
    __asm__ __volatile__( "xchgq %%rbx,%q1\n"                                          \
                          "cpuid\n"                                                    \
                          "xchgq %%rbx,%q1"                                            \
                          : "=a"( __eax ), "=r"( __ebx ), "=c"( __ecx ), "=d"( __edx ) \
                          : "0"( __func ) : )