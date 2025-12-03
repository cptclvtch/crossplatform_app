#include <stdio.h>

#include "api.c"

int main()
{
    ll_node* list = ll_add_data(NULL, PREV, NULL);

    ll_add_data(list, NEXT, NULL);

    ITERATE_LIST_START(ll_node, list, i)
        printf("\n%p\n", i);
        if(i->data == NULL) /*Oh no! We forgot to add data to this one!*/;
    ITERATE_LIST_END(NEXT, i)

    free_ll_chain(list, NEXT);
}