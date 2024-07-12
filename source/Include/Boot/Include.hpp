#pragma once
extern "C" {
#include <Guid/Acpi.h>
#include <Guid/FileInfo.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Bmp.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BmpSupportLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi.h>
}
#include <Boot/Setup.hpp>
#undef NULL
#define NULL nullptr
