#include <Arch/x86_64/platform/platform.hpp>
auto operator new( IN unsigned long int, IN QuantumNEC::Lib::Types::Ptr< VOID > addr ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID > {
    return addr;
}
auto operator new( IN unsigned long int ) noexcept -> QuantumNEC::Lib::Types::Ptr< VOID > {
    return nullptr;
}
auto operator delete( IN QuantumNEC::Lib::Types::Ptr< VOID >,
                      IN unsigned long int ) noexcept -> VOID {
}
auto operator delete( IN QuantumNEC::Lib::Types::Ptr< VOID > ) noexcept -> VOID {
}