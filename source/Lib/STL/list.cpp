#include <Arch/Arch.hpp>
#include <Lib/STL/list>
PUBLIC namespace {
    using namespace QuantumNEC;
    using Lib::STL::ListNode;
    auto list_insert_before = []( IN OUT Lib::Types::Ptr< ListNode > node, IN OUT Lib::Types::Ptr< ListNode > in_before ) -> VOID {
        in_before->prev->next = node;
        node->prev = in_before->prev;
        node->next = in_before;
        in_before->prev = node;
    };
}
PUBLIC namespace QuantumNEC::Lib::STL {
    PUBLIC auto list_init( IN OUT Lib::Types::Ptr< ListTable > list )->VOID {
        list->head.prev = NULL;
        list->head.next = &list->end;
        list->end.prev = &list->head;
        list->end.next = NULL;
    }
    PUBLIC auto list_add_to_end( IN OUT Lib::Types::Ptr< ListTable > entry, IN OUT Lib::Types::Ptr< ListNode > New )->VOID {
        list_insert_before( New, &entry->end );
    }

    PUBLIC auto list_add_to_head( IN OUT Lib::Types::Ptr< ListTable > entry, IN OUT Lib::Types::Ptr< ListNode > New )->VOID {
        list_insert_before( New, entry->head.next );
    }

    PUBLIC auto list_delete( IN OUT Lib::Types::Ptr< ListNode > entry )->VOID {
        Architecture::ArchitectureManager< TARGET_ARCH >::InterruptStatus status { Architecture::ArchitectureManager< TARGET_ARCH >::disable_interrupt( ) };
        entry->next->prev = entry->prev;
        entry->prev->next = entry->next;
        Architecture::ArchitectureManager< TARGET_ARCH >::set_interrupt( status );
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
        while ( node != &( entry->end ) ) {
            if ( node == objNode )
                return TRUE;
            node = node->next;
        }
        return FALSE;
    }
    PUBLIC auto list_length( IN Lib::Types::Ptr< ListTable > objList )->Lib::Types::size_t {
        Types::size_t length { };
        for ( auto node { objList->head.next }; node != &objList->end; ++length ) {
            node = node->next;
        }
        return length;
    }
}