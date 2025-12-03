#define INCLUDE_IMPLEMENTATION
#include "api.c"

typedef struct
{
    uint8_t a, b;
}
test_struct;

int main()
{
    chunked_list test = get_new_chunked_list(2, 2);

    add_element(&test, calloc(1, sizeof(test_struct)));
}