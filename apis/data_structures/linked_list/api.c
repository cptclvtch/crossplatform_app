//Dependencies
#include <stdlib.h>

#include "../template_header.c"
#define linked_list_node CONCATENATE(PREFIX, linked_list_node)
#define linked_list     CONCATENATE(PREFIX, linked_list)
#define add_link_before CONCATENATE(add, CONCATENATE(PREFIX, link_before))
#define add_link_after  CONCATENATE(add, CONCATENATE(PREFIX, link_after))
#define free_link       CONCATENATE(free, CONCATENATE(PREFIX, link))
#define free_link_chain CONCATENATE(free, CONCATENATE(PREFIX, link_chain))
#define create_new_list CONCATENATE(create_new, CONCATENATE(PREFIX, list))
#define free_list       CONCATENATE(free, CONCATENATE(PREFIX, list))

#ifndef _LINKED_LIST_H
    #define _LINKED_LIST_H

typedef struct linked_list_node
{
    TYPE* data;

    struct linked_list_node* PREV;
    struct linked_list_node* NEXT;
}linked_list_node;

linked_list_node* add_link_before(linked_list_node* add_to, TYPE data_in);
linked_list_node* add_link_after(linked_list_node* add_to, TYPE data_in);
void free_link(linked_list_node* to_free, void (*free_func)(void*));
void free_link_chain(linked_list_node* to_free, void (*free_func)(void*), unsigned char direction);

typedef struct linked_list
{
    linked_list_node* nodes; //FIXME is it supposed to be pointer?

    void (*free_func)();
}linked_list;

linked_list create_new_list(void (*free_func)(void*));
void free_list(linked_list* list);

void DONT_DELETE_CONTENTS(void* p){}

#endif //_LINKED_LIST_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_LINKED_LIST_C)
    #define _LINKED_LIST_C
    
#define ITERATE_LIST_START(list, iterator) \
linked_list_node* iterator = (linked_list_node*)list;\
while(iterator)\
{

#define ITERATE_LIST_END(dir, iterator)\
    iterator = iterator->dir;\
}

#define CREATE_LINK \
linked_list_node* new_node = (linked_list_node*)calloc(1, sizeof(linked_list_node));\
new_node->data = data_in;

#define get_new_link(data_in) add_link_before(NULL, data_in)

linked_list_node* add_link_before(linked_list_node* add_to, TYPE data_in)
{
    CREATE_LINK

    if(add_to)
    {
        new_node->NEXT = add_to;

        new_node->PREV = add_to->PREV;
        add_to->PREV = new_node;
    }

    return new_node;
}

linked_list_node* add_link_after(linked_list_node* add_to, TYPE data_in)
{
    CREATE_LINK

    if(add_to)
    {
        new_node->PREV = add_to;

        new_node->NEXT = add_to->NEXT;
        add_to->NEXT = new_node;
    }

    return new_node;
}
#undef CREATE_LINK

void free_link(linked_list_node* node, void (*free_func)(void*))
{
    if(node == NULL) return;
    
    if(free_func)
        free_func(node->data);
    else
        free(node->data);
    
    if(node->PREV)
        node->PREV->NEXT = node->NEXT;

    if(node->NEXT)
        node->NEXT->PREV = node->PREV;

    free(node);
}

void free_link_chain(linked_list_node* to_free, void (*free_func)(void*), unsigned char direction)
{
    linked_list_node* current_node = to_free;
    while(current_node)
    {
        linked_list_node* next_node = direction ? current_node->NEXT : current_node->PREV;
        free_link(current_node, free_func);
        current_node = next_node;
    }
}

//
linked_list create_new_list(void (*free_func)())
{
    linked_list new_list = {0};
    new_list.free_func = free_func;

    return new_list;
}

void free_list(linked_list* list)
{
    free_link_chain(list->nodes, list->free_func, 1);

    list->nodes = NULL;
}
#endif

#include "../template_footer.c"