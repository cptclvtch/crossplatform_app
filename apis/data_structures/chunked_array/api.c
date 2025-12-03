//Dependencies
#include "../bit_vector/api.c"

#include "../template_header.c"
#define chunked_array           CONCATENATE(PREFIX, chunked_array)          //TYPE_chunked_array
#define create_chunked_array    CONCATENATE(create_, chunked_array)         //create_TYPE_chunked_array
#define chunked_array_at        CONCATENATE(chunked_array, _at)             //TYPE_chunked_array_at
#define add_to_chunked_array    CONCATENATE(add_, CONCATENATE(PREFIX, to_chunked_array))    //add_TYPE_to_chunked_array
#define chunked_array_remove_range CONCATENATE(chunked_array, _remove_range)//TYPE_chunked_array_remove_range
#define chunked_array_remove    CONCATENATE(chunked_array, _remove)         //TYPE_chunked_array_remove
#define free_chunked_array      CONCATENATE(free_, chunked_array)           //free_TYPE_chunked_array

typedef struct
{
    TYPE** chunks;

    uint32_t chunk_size;
    
    bit_vector* occupancy;
}chunked_array;

chunked_array* create_TYPE_chunked_array(uint32_t chunk_size);

void* chunked_array_at(chunked_array* array, uint32_t index);
chunked_array* add_to_chunked_array(chunked_array* array, TYPE item);

chunked_array* chunked_array_remove_range(chunked_array* array, uint32_t start, uint32_t end);
chunked_array* chunked_array_remove(chunked_array* array, uint32_t index);
void free_chunked_array(chunked_array* array);

//----------------------------------
#ifdef INCLUDE_IMPLEMENTATION

chunked_array* create_chunked_array(uint32_t chunk_size, uint32_t item_size)
{
    if(!chunk_size || !item_size) return NULL;

    chunked_array* array = (chunked_array*)calloc(1, sizeof(chunked_array));
    array->chunk_size = chunk_size;
    array->chunks = calloc(1, sizeof(void*));
    array->chunks[0] = calloc(chunk_size, sizeof(TYPE));

    array->occupancy = create_bit_vector(0);

    return array;
}

TYPE chunked_array_at(chunked_array* array, uint32_t index)
{
    //sanitize args
    if(array == NULL) return NULL;
    
    if(bit_vector_get(array->occupancy, index) == 0) return (TYPE){0};

    TYPE* chunk = array->chunks[index / array->chunk_size];

    return chunk[index % array->chunk_size];
}

chunked_array* add_to_chunked_array(chunked_array* array, TYPE item)
{
    if(array == NULL) return NULL;

    //find first empty slot
    uint32_t slot = 0;
    while(bit_vector_get(array->occupancy, slot)){slot++;}

    if(slot > array->occupancy->last_index)
    {
        //add new slot
        void* new_chunks = realloc(array->chunks, array->occupancy->last_index / array->chunk_size + 1);
        if(new_chunks == NULL) return array;

        if(new_chunks != array->chunks) free(array->chunks);
        array->chunks = new_chunks;

        bit_vector_push_last(array->occupancy, 0);
    }

    TEST_PRINT("slot: %u", slot);
    ITEM_AT(array, slot) = item;
    bit_vector_set(array->occupancy, slot, 1);

    return array;
}

chunked_array* chunked_array_remove_range(chunked_array* array, uint32_t start, uint32_t end)
{
    if(array == NULL) return NULL;
    if(start > end)
    {
        uint32_t temp = start;
        start = end; end = temp;
    }

    uint32_t i = start;
    for(;i <= end; i++)
    {
        bit_vector_set(array->occupancy, i, 0);
    }

    //TODO downsize if chunks at the end have no occupancy
}

chunked_array* chunked_array_remove(chunked_array* array, uint32_t index)
{
    return chunked_array_remove_range(array, index, index);
}

void free_chunked_array(chunked_array* array)
{
    if(array == NULL) return;

    uint32_t chunk = 0;
    for(; chunk < array->occupancy->last_index / array->chunk_size; chunk++)
    {
        free(array->chunks[chunk]);
    }

    free(array->chunks);
    free_bit_vector(array->occupancy);
    free(array);
}
#endif

#undef chunked_array
#undef create_chunked_array
#undef chunked_array_at
#undef add_to_chunked_array
#undef chunked_array_remove_range
#undef chunked_array_remove
#undef free_chunked_array
#include "../template_footer.c"