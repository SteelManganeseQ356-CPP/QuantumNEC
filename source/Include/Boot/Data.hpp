#pragma once
extern "C" {
#include <Library/UefiLib.h>
#include <Uefi.h>
}
#include <concepts>
#define _packed __attribute__( ( packed ) )
namespace QuantumNEC::Boot {
namespace {
EFI_HANDLE GlobalImageHandle { };

EFI_SYSTEM_TABLE *GlobalSystemTable { };
}     // namespace
/**
 * @brief 管理启动时服务产生的数据
 * @tparam T 要管理的数据，约定是必须有set，put两大成员函数
 */
template < class T >
    requires requires( T test ) {
        { test.set( ) } -> std::same_as< VOID >;     // 调用约定，以后需要
        { test.put( ) } -> std::same_as< VOID >;
    }
class BootServiceDataManager
{
public:
    explicit BootServiceDataManager( CONST T *_data ) noexcept( true ) :
        data_ { *_data } {
    }
    virtual ~BootServiceDataManager( VOID ) noexcept( true ) = default;

public:
    auto put( VOID ) -> T & {
        return this->data_;
    }
    auto putSTable( VOID ) -> EFI_SYSTEM_TABLE * {
        return GlobalSystemTable;
    }
    auto getSTable( IN EFI_SYSTEM_TABLE *_systemTable ) -> VOID {
        GlobalSystemTable = _systemTable;
        return;
    }
    auto putHandle( VOID ) -> EFI_HANDLE {
        return GlobalImageHandle;
    }
    auto getHandle( IN EFI_HANDLE _efiHandle ) -> VOID {
        GlobalImageHandle = _efiHandle;
        return;
    }

private:
    T data_ { };
};

}     // namespace QuantumNEC::Boot