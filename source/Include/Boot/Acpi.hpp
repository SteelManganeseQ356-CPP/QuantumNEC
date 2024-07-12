#pragma once
#include <Boot/base.hpp>
namespace QuantumNEC::Boot {
class AcpiService
{
public:
    explicit AcpiService( VOID ) noexcept;
    virtual ~AcpiService( VOID ) noexcept;

public:
    auto get_acpi_table( VOID ) noexcept {
        return this->acpi_table;
    }

private:
    VOID *acpi_table;
};

}     // namespace QuantumNEC::Boot