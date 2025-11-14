#include <stdio.h>

#define INCLUDE_IMPLEMENTATION

#define TYPE char
#include "api.c"

#include "api.c"

#define TYPE char*
#define DESIRED_PREFIX charp
#include "api.c"

int main()
{
    char_cyclical_array* my_array = create_char_cyclical_array(2);
    cyclical_array* another_array = create_cyclical_array(2);

    printf("void*: %p\n", another_array->data);
    printf("String: %u\n",my_array->data);

    free_cyclical_array(another_array);
    free_char_cyclical_array(my_array);
}