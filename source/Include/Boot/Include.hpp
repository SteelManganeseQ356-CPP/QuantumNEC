#pragma once
extern "C" {
#include <Guid/Acpi.h>
#include <Guid/FileInfo.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Bmp.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BmpSupportLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi.h>
}
#include <Boot/Setup.hpp>
#undef NULL
#define NULL nullptr
constexpr CONST auto KERNEL_PHYSICAL_ADDRESS { 0x100000ULL };
constexpr CONST auto KERNEL_VRAM_PHYSICAL_ADDRESS { 0x800000 };
constexpr CONST auto KERNEL_FONT_MEMORY_PHYSICAL_ADDRESS { 0x0a00000 };
constexpr CONST auto KERNEL_FREE_MEMORY_PHYSICAL_ADDRESS { 0x2a00000 };
constexpr CONST auto KERNEL_TASK_PCB_PHYSICAL_ADDRESS { 0x0200000 };
constexpr CONST auto KERNEL_PAGE_DIRECTORY_PHYSICAL_ADDRESS { 0x0600000 };
constexpr CONST auto KERNEL_I_O_APIC_PHYSICAL_ADDRESS { 0x0210000 };
constexpr CONST auto KERNEL_LOCAL_APIC_PHYSICAL_ADDRESS { 0x0211000 };
constexpr CONST auto KERNEL_PAGE_TABLE_RESERVED_PHYSICAL_ADDRESS { 0x0212000 };