//Dependencies
#include <stdint.h>
#include <stdlib.h>

#include "../template_header.c"
#define cyclical_array CONCATENATE(PREFIX, cyclical_array)
#define create_array CONCATENATE(create_, cyclical_array)
#define free_array CONCATENATE(free_, cyclical_array)

typedef struct
{
    TYPE* data;

    uint32_t max_index;
}cyclical_array;

cyclical_array* create_array(uint32_t last_index);
void free_array(cyclical_array* a);

//----------------------------------
#ifdef INCLUDE_IMPLEMENTATION

cyclical_array* create_array(uint32_t max_index)
{
    cyclical_array* new_array = calloc(1, sizeof(cyclical_array));

    #ifndef USING_VOIDP_DEFAULT
    new_array->data = calloc(max_index+1, sizeof(TYPE)); //FIXME not zeroing?
    #endif
    new_array->max_index = max_index;

    return new_array;
}

void free_array(cyclical_array* a)
{
    free(a->data);
    free(a);
}

#endif

#undef cyclical_array
#undef create_array
#undef free_array
#include "../template_footer.c"