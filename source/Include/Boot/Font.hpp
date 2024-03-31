#pragma once
#include <Boot/Data.hpp>
namespace QuantumNEC::Boot {
typedef struct
{
    VOID *fontBuffer;
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} UnicodeTTF;
class BootServiceFont : protected BootServiceDataManager< UnicodeTTF >
{
public:
    explicit BootServiceFont( IN UnicodeTTF * ) noexcept( true );
    virtual ~BootServiceFont( VOID ) noexcept( true ) = default;

public:
    auto getUnicodeTTF( IN CONST wchar_t *fontPath ) -> EFI_STATUS;
};
}     // namespace QuantumNEC::Boot