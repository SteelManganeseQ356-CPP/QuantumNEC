extern "C" {
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
}
#undef NULL
#define NULL nullptr
#include <cstddef>
#include <cstdint>
namespace {
inline EFI_HANDLE GlobalImageHandle { };

inline EFI_SYSTEM_TABLE *GlobalSystemTable { };
}     // namespace