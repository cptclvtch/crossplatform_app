//Dependencies
#include "../bit_vector/api.c"

#ifndef _CHUNKED_ARRAY_H
    #define _CHUNKED_ARRAY_H

#ifndef FORCE_INLINE
#define FORCE_INLINE __attribute((always_inline)) inline
#endif

typedef struct
{
    uint8_t** chunks;

    uint32_t chunk_size;
    uint32_t item_size;
    
    bit_vector* occupancy;
}chunked_array;

chunked_array* create_chunked_array(uint32_t chunk_size, uint32_t item_size);

void* chunked_array_at(chunked_array* array, uint32_t index);
chunked_array* chunked_array_add(chunked_array* array, void* item);

chunked_array* chunked_array_remove_range(chunked_array* array, uint32_t start, uint32_t end);
chunked_array* chunked_array_remove(chunked_array* array, uint32_t index);
void free_chunked_array(chunked_array* array);

#endif //_CHUNKED_ARRAY_H

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_CHUNKED_ARRAY_C)
    #define _CHUNKED_ARRAY_C

#define ITEM_AT(a, i) (*(a->chunks[i / a->chunk_size] + a->item_size*(i % a->chunk_size)))

chunked_array* create_chunked_array(uint32_t chunk_size, uint32_t item_size)
{
    if(!chunk_size || !item_size) return NULL;

    chunked_array* array = (chunked_array*)calloc(1, sizeof(chunked_array));
    array->chunk_size = chunk_size;
    array->item_size = item_size;
    array->chunks = calloc(1, sizeof(void*));
    array->chunks[0] = calloc(chunk_size, item_size);

    array->occupancy = create_bit_vector(0);

    return array;
}

void* chunked_array_at(chunked_array* array, uint32_t index)
{
    //sanitize args
    if(array == NULL) return NULL;
    
    if(bit_vector_get(array->occupancy, index) == 0) return NULL;

    return ITEM_AT(array, index);
}

chunked_array* chunked_array_add(chunked_array* array, void* item)
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
#endif // _CHUNKED_ARRAY_C