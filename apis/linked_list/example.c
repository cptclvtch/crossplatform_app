#include "api.c"
#define API_IMPLEMENTATION_ONLY
#include "api.c"

int main()
{
    linked_list list = (linked_list){0};
    list.nodes = add_link_before(NULL, NULL);

    add_link_after(list, NULL);

    ITERATE_LIST_START(list, i)
        if(i->data == NULL) /*Oh no! We forgot to add data to this one!*/;
    ITERATE_LIST_END(NEXT, i)

}