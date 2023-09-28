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
    explicit BootServiceFont( UnicodeTTF * );
    virtual ~BootServiceFont( VOID ) = default;

public:
    auto getUnicodeTTF( IN CONST wchar_t *fontPath ) -> EFI_STATUS;
};
}     // namespace QuantumNEC::Boot