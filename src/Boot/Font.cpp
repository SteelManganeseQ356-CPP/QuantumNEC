#include <Boot/Font.hpp>
namespace QuantumNEC::Boot {
BootServiceFont::BootServiceFont( UnicodeTTF *config ) :
    BootServiceDataManage< UnicodeTTF > { config } {
        
}
auto BootServiceFont::setUnicodeTTF( VOID ) -> EFI_STATUS {
    return EFI_SUCCESS;
}
}     // namespace QuantumNEC::Boot