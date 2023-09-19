#pragma once
#include <Boot/Data.hpp>
namespace QuantumNEC::Boot {
typedef struct
{
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} UnicodeTTF;
class BootServiceFont : protected BootServiceDataManage< UnicodeTTF >
{
public:
    explicit BootServiceFont( UnicodeTTF * );
    virtual ~BootServiceFont( VOID ) = default;

public:
    auto setUnicodeTTF( ) -> EFI_STATUS;
};
}     // namespace QuantumNEC::Boot