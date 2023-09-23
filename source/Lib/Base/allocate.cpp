#include <Lib/Base/allocate.hpp>
PUBLIC namespace QuantumNEC::Lib::Base {
    AllocateManagement::AllocateManagement(
        IN Lib::Types::Ptr< AllocateTable > _table,
        IN Lib::Types::Ptr< AllocateTableEntry > entries,
        IN Lib::Types::uint64_t entriesCount ) noexcept :
        table_ { _table } {
        this->table_->entriesCount = entriesCount;
        this->table_->frees = 0;
        this->table_->entries = entries;
    }
    AllocateManagement::AllocateManagement( IN CONST Lib::Types::L_Ref< AllocateManagement > _alloc_table ) noexcept {
        this->table_ = _alloc_table.table_;
    }
    auto AllocateManagement::operator=( IN CONST Lib::Types::L_Ref< AllocateManagement > _alloc_table ) noexcept -> CONST Lib::Types::L_Ref< AllocateManagement > {
        this->table_ = _alloc_table.table_;
        return *this;
    }
    auto AllocateManagement::alloc( IN Types::uint64_t unitsCount )->Types::int64_t {
        Lib::Types::uint64_t index { };
       
        for ( Lib::Types::uint64_t i { }; i < this->table_->entriesCount; i++ ) {
                
            if ( this->table_->entries[ i ].unitsCount >= unitsCount ) {
                index = this->table_->entries[ i ].index;
                this->table_->entries[ i ].index += unitsCount;
                this->table_->entries[ i ].unitsCount -= unitsCount;
                if ( !(this->table_->entries[ i ].unitsCount) ) {
                    this->table_->frees--;
                    while ( i < this->table_->frees ) {
                        this->table_->entries[ i ] = this->table_->entries[ i + 1 ];
                        i++;
                    }
                }
                return index;
            }
        }
        return -1;
    }
    auto AllocateManagement::free( IN Types::uint64_t index, IN Types::uint64_t unitsCount )->VOID {
        Lib::Types::uint64_t i { }, j { };
        while ( i < this->table_->frees ) {
            if ( this->table_->entries[ i ].index > index ) {
                break;
            }
            ++i;
        }
        if ( i > 0 ) {
            if ( this->table_->entries[ i - 1 ].index + this->table_->entries[ i - 1 ].unitsCount == index ) {
                this->table_->entries[ i - 1 ].unitsCount += unitsCount;
                if ( i < this->table_->frees ) {
                    if ( index + unitsCount == this->table_->entries[ i ].index ) {
                        this->table_->entries[ i - 1 ].unitsCount +=
                            this->table_->entries[ i ].unitsCount;
                        this->table_->frees--;
                        while ( i < this->table_->frees ) {
                            this->table_->entries[ i ] = this->table_->entries[ i + 1 ];
                            i++;
                        }
                    }
                }
                return;
            }
        }
        if ( i < this->table_->frees ) {
            if ( index + unitsCount == this->table_->entries[ i ].index ) {
                this->table_->entries[ i ].index = index;
                this->table_->entries[ i ].unitsCount += unitsCount;
                return;
            }
        }
        if ( this->table_->frees < this->table_->entriesCount ) {
            for ( j = this->table_->frees; j > i; --j ) {
                this->table_->entries[ j ] = this->table_->entries[ j - 1 ];
            }
            this->table_->frees++;
            this->table_->entries[ i ].index = index;
            this->table_->entries[ i ].unitsCount = unitsCount;
            return;
        }
        return;
    }
    auto AllocateManagement::total( VOID )->Lib::Types::uint64_t {
        Lib::Types::uint64_t i;
        Lib::Types::uint64_t free_units = 0;
        for ( i = 0; i < this->table_->frees; i++ ) {
            free_units += this->table_->entries[ i ].unitsCount;
        }
        return free_units;
    }
}