#pragma once
#include <Boot/base.hpp>
#include <cstdint>
namespace QuantumNEC::Boot {
class FileService
{
public:
    explicit FileService( IN CHAR16 *path ) noexcept;
    virtual ~FileService( VOID ) noexcept;

public:
    auto read( VOID ) -> std::uint64_t;

private:
    EFI_FILE_PROTOCOL *file_protocol;
};
}     // namespace QuantumNEC::Boot