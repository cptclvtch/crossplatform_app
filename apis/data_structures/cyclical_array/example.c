#define INCLUDE_IMPLEMENTATION
#define TYPE char
#include "api.c"

#include "api.c"

int main()
{
    char_cyclical_array* my_array = create_char_cyclical_array(2);
    a_cyclical_array* another_array = create_a_cyclical_array(2);

    free_a_cyclical_array(another_array);
    free_char_cyclical_array(my_array);
}