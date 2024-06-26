#pragma once
#include <Boot/Data.hpp>
#include <Boot/Logger.hpp>
namespace QuantumNEC::Boot {
typedef struct
{
    VOID *data_ { };
    UINT64 pageNumber_ { };
    UINT8 *ptr_ { };
    auto set( VOID ) -> VOID {
    }
    auto put( VOID ) -> VOID {
    }
} ArgsStackConfig;

class BootServiceArgs : protected BootServiceDataManager< ArgsStackConfig >
{
public:
    explicit BootServiceArgs( IN ArgsStackConfig *config, IN CONST UINT64 size ) noexcept( true );
    virtual ~BootServiceArgs( VOID ) noexcept( true ) = default;

public:
    auto argsPush( IN OUT VOID **object, IN UINT64 size ) -> EFI_STATUS;
};
}     // namespace QuantumNEC::Boot