#include <Boot/Data.hpp>
#include <Boot/Utils.hpp>
#include <Boot/Include.hpp>
namespace QuantumNEC::Boot {

bool PowerU64( IN UINT64 X, IN UINTN Y ) {
    if ( !X || !Y ) {
        return !X ? false : true;
    }
    UINT64 Result { X };
    while ( --Y ) {
        Result *= X;
    }
    return Result;
}
}     // namespace QuantumNEC::Boot
VOID operator delete( VOID *, size_t ) noexcept {
    return;
}
VOID operator delete( VOID *address ) noexcept {
    if ( !address )
        return;
    gBS->FreePool( address );
    return;
}
VOID operator delete[]( VOID *address ) noexcept {
    operator delete( address );
    return;
}
VOID *operator new( size_t size ) noexcept {
    if ( size < 1 ) {
        return nullptr;
    }
    VOID *RetVal { };
    if ( gBS->AllocatePool( EfiLoaderData, static_cast< UINTN >( size ), &RetVal ) != EFI_SUCCESS ) {
        return nullptr;
    }
    return RetVal;
}
VOID *operator new[]( size_t cb ) noexcept {
    return operator new( cb );
}
VOID *operator new( size_t, VOID *address ) noexcept {
    return address;
}