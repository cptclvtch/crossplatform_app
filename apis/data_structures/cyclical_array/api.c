//Dependencies
// #include <stdint.h>
// #include <stdlib.h>

#include "../template_header.c"
#define cyclical_array CONCATENATE(PREFIX, cyclical_array)
#define create_array CONCATENATE(create, cyclical_array)
#define free_array CONCATENATE(free, cyclical_array)

#if !defined(_CYCLICAL_ARRAY_H) && !defined(TYPE) //FIXME nah, this wont work, feels close though!
    #define _CYCLICAL_ARRAY_H

typedef struct
{
    #ifdef USING_VOIDP_DEFAULT
    void* data;
    #else
    TYPE* data;
    #endif

    uint32_t max_index;
}cyclical_array;

cyclical_array* create_array(uint32_t last_index);
void free_array(cyclical_array* a);

#endif //_CYCLICAL_ARRAY_H

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_CYCLICAL_ARRAY_C##TYPE)
    #define _CYCLICAL_ARRAY_C##TYPE

cyclical_array* create_array(uint32_t max_index)
{
    cyclical_array* new_array = calloc(1, sizeof(cyclical_array));

    new_array->data = calloc(max_index+1, sizeof(TYPE));
    new_array->max_index = max_index;

    return new_array;
}

void free_array(cyclical_array* a)
{
    free(a->data);
    free(a);
}

#endif //_CYCLICAL_ARRAY_C

#undef cyclical_array
#undef create_array
#undef free_array
#include "../template_footer.c"