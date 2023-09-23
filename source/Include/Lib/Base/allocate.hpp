#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Lib::Base {
    PUBLIC typedef struct
    {
        Lib::Types::uint64_t index;          // 该条目的索引
        Lib::Types::uint64_t unitsCount;     // 该条目包含几个空置单元
    } AllocateTableEntry;

    PUBLIC typedef struct
    {
        Lib::Types::uint64_t entriesCount;                 // 条目总数
        Lib::Types::uint64_t frees;                        // 空闲条目数
        Lib::Types::Ptr< AllocateTableEntry > entries;     // 条目内容
    } AllocateTable;
    PUBLIC class AllocateManagement
    {
    public:
        explicit( TRUE )
            AllocateManagement(
                IN Lib::Types::Ptr< AllocateTable > _table,
                IN Lib::Types::Ptr< AllocateTableEntry > entries,
                IN Lib::Types::uint64_t entriesCount ) noexcept;
        explicit( TRUE ) AllocateManagement( IN CONST Lib::Types::L_Ref< AllocateManagement > ) noexcept;
        CONST Lib::Types::L_Ref< AllocateManagement >
        operator=( IN CONST Lib::Types::L_Ref< AllocateManagement > ) noexcept;
        virtual ~AllocateManagement( VOID ) noexcept = default;

    public:
        /**
         * @brief 在表中分配单元
         * @param table 从这个表中分配
         * @param unitsCount 分配的单元个数
         * @return 分配到的单元下标
         */
        auto alloc( IN Types::uint64_t unitsCount ) -> Types::int64_t;
        /**
         * @brief 在表中释放单元
         * @param table 从这个表中释放
         * @param index 单元编号
         * @param unitsCount 释放的单元个数
         */
        auto free( IN Types::uint64_t index, IN Types::uint64_t unitsCount ) -> VOID;
        /**
         * @brief 统计空闲单元
         * @return 单元个数
        */
        auto total( VOID ) -> Lib::Types::uint64_t;

    public:
        /**
         * @brief 更换分配表对象
         * @param changeObject 要更换的分配表地址
         */
        auto
        changeAllocateTable( IN Lib::Types::Ptr< AllocateTable > changeObject ) -> VOID {
            this->table_ = changeObject;
        }

    private:
        Lib::Types::Ptr< AllocateTable > table_ { };
    };
}