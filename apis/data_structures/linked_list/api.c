//Dependencies
#include <stdlib.h>

#include "../template_header.c"
#define ll_node         CONCATENATE(PREFIX, ll_node)                        //TYPE_ll_node
#define ll_add_data     CONCATENATE(ll_add_, CONCATENATE(PREFIX, data))     //ll_add_TYPE_data
#define ll_add_link     CONCATENATE(ll_add_, CONCATENATE(PREFIX, link))     //ll_add_TYPE_link
#define create_ll_node  CONCATENATE(create_, ll_node)                       //create_TYPE_ll_node
#define free_ll_node    CONCATENATE(free_, ll_node)                         //free_TYPE_ll_node
#define free_ll_chain   CONCATENATE(free_, CONCATENATE(PREFIX, ll_chain))   //free_TYPE_ll_chain

typedef struct ll_node
{
    TYPE data;

    union
    {
        struct ll_node* link[2];

        struct
        {
            struct ll_node* PREV;
            struct ll_node* NEXT;
        };
    };
}ll_node;

ll_node* ll_add_data(ll_node* add_to, unsigned char direction, TYPE data_in);
ll_node* ll_add_link(ll_node* add_to, unsigned char direction, ll_node* link_in);
ll_node* create_ll_node(TYPE data_in);
void free_ll_node(ll_node* to_free);
void free_ll_chain(ll_node* to_free, unsigned char direction);

#ifndef _LINKED_LIST_ONCE
#define _LINKED_LIST_ONCE

#define ITERATE_LIST_START(node_type, root, iterator) \
node_type* iterator = root;\
while(iterator)\
{

#define ITERATE_LIST_END(dir, iterator)\
    iterator = iterator->dir;\
}

enum
{
    PREV,
    NEXT
};
#endif

//----------------------------------
#ifndef HEADERS_ONLY
ll_node* ll_add_data(ll_node* add_to, unsigned char direction, TYPE data_in)
{
    ll_node* new_node = (ll_node*)calloc(1, sizeof(ll_node));
    new_node->data = data_in;

    if(add_to)
    {
        new_node->link[1-direction] = add_to;

        new_node->link[direction] = add_to->link[direction];
        add_to->link[direction] = new_node;
    }

    return new_node;
}

ll_node* ll_add_link(ll_node* add_to, unsigned char direction, ll_node* link_in)
{
    if(link_in == NULL)
        link_in = (ll_node*)calloc(1, sizeof(ll_node));

    if(add_to)
    {
        link_in->link[1-direction] = add_to;

        link_in->link[direction] = add_to->link[direction];
        add_to->link[direction] = link_in;
    }

    return link_in;
}

ll_node* create_ll_node(TYPE data_in) { return ll_add_data(NULL, PREV, data_in); }

void free_ll_node(ll_node* node)
{
    if(node == NULL) return;
    
    if(node->PREV)
        node->PREV->NEXT = node->NEXT;

    if(node->NEXT)
        node->NEXT->PREV = node->PREV;

    free(node);
}

void free_ll_chain(ll_node* to_free, unsigned char direction)
{
    ll_node* current_node = to_free;
    while(current_node)
    {
        ll_node* next_node = current_node->link[direction];
        free_ll_node(current_node);
        current_node = next_node;
    }
}
#endif

#undef ll_node
#undef ll_add_data
#undef ll_add_link
#undef create_ll_node
#undef free_ll_node
#undef free_ll_chain
#include "../template_footer.c"