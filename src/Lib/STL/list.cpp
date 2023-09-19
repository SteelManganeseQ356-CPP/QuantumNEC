#include <Lib/STL/list>
PUBLIC namespace QuantumNEC::Lib::STL {
    PUBLIC auto list_init( IN OUT Lib::Types::Ptr< ListTable > list )->VOID {
        list->head.prev = NULL;
        list->head.next = &list->end;
        list->end.prev = &list->head;
        list->end.next = NULL;
    }
    PRIVATE inline auto list_insert_before( IN OUT Lib::Types::Ptr< ListNode > node, IN OUT Lib::Types::Ptr< ListNode > in_before )->VOID {
        in_before->prev->next = node;
        node->prev = in_before->prev;
        node->next = in_before;
        in_before->prev = node;
    }
    PUBLIC auto list_add_to_end( IN OUT Lib::Types::Ptr< ListTable > entry, IN OUT Lib::Types::Ptr< ListNode > New )->VOID {
        list_insert_before( New, &entry->end );
    }

    PUBLIC auto list_add_to_head( IN OUT Lib::Types::Ptr< ListTable > entry, IN OUT Lib::Types::Ptr< ListNode > New )->VOID {
        list_insert_before( New, entry->head.next );
    }

    PUBLIC auto list_delete( IN OUT Lib::Types::Ptr< ListNode > entry )->VOID {
        // IntrStatus Status{ intr_disable () };
        entry->next->prev = entry->prev;
        entry->prev->next = entry->next;
        // intr_set_status (Status);
    }
    PUBLIC auto list_pop( IN OUT Lib::Types::Ptr< ListTable > entry )->Lib::Types::Ptr< ListNode > {
        Lib::Types::Ptr< ListNode > node { entry->head.next };
        list_delete( node );
        return node;
    }
    PUBLIC auto list_is_empty( IN Lib::Types::Ptr< ListTable > entry )->Lib::Types::BOOL {
        return entry->head.next == &entry->end ? TRUE : FALSE;
    }

    PUBLIC auto list_prev( IN Lib::Types::Ptr< ListNode > node )->Lib::Types::Ptr< ListNode > {
        return node->prev ? node->prev : NULL;
    }

    PUBLIC auto list_next( IN Lib::Types::Ptr< ListNode > node )->Lib::Types::Ptr< ListNode > {
        return node->next ? node->next : NULL;
    }
    PUBLIC auto list_find( IN Lib::Types::Ptr< ListTable > entry, IN Lib::Types::Ptr< ListNode > objNode )->Lib::Types::BOOL {
        ListNode near *node { entry->head.next };
        while ( node != &( entry->end ) )
        {
            if ( node == objNode )
                return TRUE;
            node = node->next;
        }
        return FALSE;
    }
    PUBLIC auto list_traversal( IN OUT Lib::Types::Ptr< ListTable > objList, IN CONST Lib::Types::FuncPtr< Lib::Types::BOOL, Lib::Types::Ptr< ListNode >, Lib::Types::int32_t > func, IN CONST Lib::Types::int32_t arg )
        ->Lib::Types::Ptr< ListNode > {
        Lib::Types::Ptr< ListNode > node { objList->head.next };
        /* 如果队列为空，就必然没有符合条件的结点，故直接返回 NULL */
        if ( list_is_empty( objList ) )
            return NULL;
        while ( node != &objList->end )
        {
            if ( func(
                     node,
                     arg ) )     // func 返回
                                 // ture，则认为该元素在回调函数中符合条件，命中，故停止继续遍历
                return node;
            // 若回调函数 func 返回 true，则继续遍历
            node = node->next;
        }
        return NULL;
    }
    PUBLIC auto list_length( IN Lib::Types::Ptr< ListTable > objList )->Lib::Types::size_t {
        Types::size_t length { };
        for ( auto node { objList->head.next }; node != &objList->end; ++length )
        {
            node = node->next;
        }
        return length;
    }
}