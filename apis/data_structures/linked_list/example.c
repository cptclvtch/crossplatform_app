#include <stdio.h>

#define INCLUDE_IMPLEMENTATION
#include "api.c"

int main()
{
    linked_list list = (linked_list){0};
    list.nodes = add_link_before(NULL, NULL);

    add_link_after((linked_list_node*)list, NULL);

    ITERATE_LIST_START(list, i)
        printf("\n%p\n", i);
        if(i->data == NULL) /*Oh no! We forgot to add data to this one!*/;
    ITERATE_LIST_END(NEXT, i)

    free_list(&list);
}