#ifndef API_IMPLEMENTATION_ONLY
#define LINKED_LIST

#ifndef RAND_MAX
#include <stdlib.h>
#endif

typedef struct linked_list_node
{
    void* data;

    struct linked_list_node* PREV;
    struct linked_list_node* NEXT;
}linked_list_node;

linked_list_node* add_link_before(linked_list_node* add_to, void* data_in);
linked_list_node* add_link_after(linked_list_node* add_to, void* data_in);
void delete_link(linked_list_node* to_delete, void (*delete_func)(void*));

typedef struct linked_list
{
    linked_list_node* nodes;

    void (*delete_func)();
}linked_list;

linked_list create_new_list(void (*delete_func)(void*));
void delete_list(linked_list* list);

//----------------------------------
#else
//----------------------------------

#define ITERATE_LIST_START(list, iterator) \
linked_list_node* iterator = list.nodes;\
while(iterator)\
{

#define ITERATE_LIST_END(dir, iterator)\
    iterator = iterator->dir;\
}

#define CREATE_LINK \
linked_list_node* new_node = (linked_list_node*)calloc(1, sizeof(linked_list_node));\
new_node->data = data_in;

#define get_new_link(data_in) add_link_before(NULL, data_in)

linked_list_node* add_link_before(linked_list_node* add_to, void* data_in)
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

linked_list_node* add_link_after(linked_list_node* add_to, void* data_in)
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

void _dont_delete_contents(void* p){}
#define DONT_DELETE_CONTENTS &_dont_delete_contents

void delete_link(linked_list_node* node, void (*delete_func)(void*))
{
    if(node == NULL) return;
    
    if(delete_func)
        delete_func(node->data);
    else
        free(node->data);
    
    if(node->PREV)
        node->PREV->NEXT = node->NEXT;

    if(node->NEXT)
        node->NEXT->PREV = node->PREV;

    free(node);
}

//
linked_list create_new_list(void (*delete_func)())
{
    linked_list new_list = {0};
    new_list.delete_func = delete_func;

    return new_list;
}

void delete_list(linked_list* list)
{
    linked_list_node* current_node = list->nodes;
    while(current_node)
    {
        linked_list_node* next_node = current_node->NEXT;
        delete_link(current_node, list->delete_func);
        current_node = next_node;
    }

    list->nodes = NULL;
}
#endif