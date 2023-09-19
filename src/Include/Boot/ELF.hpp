#pragma once
#include <Boot/Data.hpp>
#include <Boot/File.hpp>
#include <Boot/Memory.hpp>
namespace QuantumNEC::Boot {
constexpr CONST auto ELF_64 { 2 };
constexpr CONST auto NOT_64_BIT { -2 };
constexpr CONST auto NOT_ELF { -1 };
constexpr CONST auto ELF_32 { 1 };
constexpr CONST auto PT_LOAD { 1 };
// ELF头魔术字节
constexpr CONST auto ELF_MAGIC { 0x464c457F };
constexpr CONST auto ELFCLASSNONE { 0 };
constexpr CONST auto ELFCLASS32 { 1 };
constexpr CONST auto ELFCLASS64 { 2 };
constexpr CONST auto ELFDATANONE { 0 };
constexpr CONST auto ELFDATA_LSB { 1 };
constexpr CONST auto ELFDATA_MSB { 2 };
constexpr CONST auto EV_NONE { 0 };
constexpr CONST auto EV_CURRENT { 1 };
constexpr CONST auto ELFOSABI_NONE { 0 };
constexpr CONST auto ELFOSABI_LINUX { 3 };
constexpr CONST auto ELFRESERVED { 0 };
constexpr CONST auto EM_386 { 3 };
constexpr CONST auto EM_X86_64 { 62 };
constexpr CONST auto EM_ARM { 40 };
constexpr CONST auto EM_AARCH64 { 183 };
constexpr CONST auto ET_NONE { 0 };
constexpr CONST auto ET_REL { 1 };
constexpr CONST auto ET_EXEC { 2 };
constexpr CONST auto ET_DYN { 3 };
#pragma pack( 1 )
typedef struct ElfHeader32
{
    UINT32 Magic;           // 0x00
    UINT8 Format;           // 0x04,32 or 64 bits format 1=32, 2=64
    UINT8 Endianness;       // 0x05
    UINT8 Version;          // 0x06
    UINT8 OSAbi;            // 0x07
    UINT8 AbiVersion;       // 0x08
    UINT8 Rserved[ 7 ];     // 0x09
    UINT16 Type;            // 0x10
    UINT16 Machine;         // 0x12
    UINT32 ElfVersion;      // 0x14
    UINT32 Entry;           // 0x18
    UINT32 Phoff;           // 0x1C
    UINT32 Shoff;           // 0x20
    UINT32 Flags;           // 0x24
    UINT16 HeadSize;        // 0x28,size of elf head
    UINT16 PHeadSize;       // 0x2A, size of a program header table entry
    UINT16 PHeadCount;      // 0x2C, count of entries in the program header table
    UINT16 SHeadSize;       // 0x2E, size of a section header table entry
    UINT16 SHeadCount;      // 0x30, count of entries in the section header table
    UINT16 SNameIndex;      // 0x32, index of entry that contains the section names
} ElfHeader32;
typedef struct ElfHeader64
{
    UINT32 Magic;           // 0x00
    UINT8 Format;           // 0x04,32 or 64 bits format
    UINT8 Endianness;       // 0x05
    UINT8 Version;          // 0x06
    UINT8 OSAbi;            // 0x07
    UINT8 AbiVersion;       // 0x08
    UINT8 Rserved[ 7 ];     // 0x09
    UINT16 Type;            // 0x10
    UINT16 Machine;         // 0x12
    UINT32 ElfVersion;      // 0x14
    UINT64 Entry;           // 0x18
    UINT64 Phoff;           // 0x20
    UINT64 Shoff;           // 0x28
    UINT32 Flags;           // 0x30
    UINT16 HeadSize;        // 0x34,size of elf head
    UINT16 PHeadSize;       // 0x36, size of a program header table entry
    UINT16 PHeadCount;      // 0x38, count of entries in the program header table
    UINT16 SHeadSize;       // 0x3A, size of a section header table entry
    UINT16 SHeadCount;      // 0x3C, count of entries in the section header table
    UINT16 SNameIndex;      // 0x3E, index of entry that contains the section names
} ElfHeader64;

typedef struct ProgramHeader32
{
    UINT32 Type;             // 0x00, type of segment
    UINT32 Offset;           // 0x04, Offset of the segment in the file image
    UINT32 VAddress;         // 0x08, Virtual address of the segment in memory
    UINT32 PAddress;         // 0x0C, reserved for segment`s physical address
    UINT32 SizeInFile;       // 0x10, size in bytes of the segment in the file image
    UINT32 SizeInMemory;     // 0x14, size in bytes of the segment in memory
    UINT32 Flags;            // 0x18
    UINT32 Align;            // 0x1C, 0 and 1
} ProgramHeader32;

typedef struct ProgramHeader64
{
    UINT32 Type;             // 0x00, type of segment
    UINT32 Flags;            // 0x04 Segment-dependent flags
    UINT64 Offset;           // 0x08
    UINT64 VAddress;         // 0x10
    UINT64 PAddress;         // 0x18
    UINT64 SizeInFile;       // 0x20
    UINT64 SizeInMemory;     // 0x28
    UINT64 Align;            // 0x30
} ProgramHeader64;
#pragma pack( )
/* Mutiple arch but not used now */
#if defined( MDE_CPU_X64 ) || defined( MDE_CPU_AARCH64 )
using ElfProgramHeader = ProgramHeader64;
#elif defined( MDE_CPU_IA32 ) || defined( MDE_CPU_ARM )
using ElfProgramHeader = ProgramHeader32;
#endif
class BootServiceELF
{
public:
    explicit BootServiceELF( VOID );
    virtual ~BootServiceELF( VOID ) = default;

protected:
    EFI_PHYSICAL_ADDRESS address { };

private:
    BootServiceFile fileUtils;

private:
    auto CheckELF( IN EFI_PHYSICAL_ADDRESS KernelBuffer ) -> EFI_STATUS;

    auto LoadSegments( IN EFI_PHYSICAL_ADDRESS KernelBufferBase ) -> EFI_STATUS;

public:
    auto loadKernel( IN CONST wchar_t *kernelPath ) -> EFI_STATUS;
};
}     // namespace QuantumNEC::Boot