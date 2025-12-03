//Dependencies
#include <stdint.h>
#include <stdlib.h>

#ifndef _BIT_VECTOR_H
    #define _BIT_VECTOR_H

// #define EMPTY_BIT_VECTOR (-1)

typedef struct
{
    uint8_t* chunks;
    uint32_t last_index;

    uint32_t set_bit_count;
}bit_vector;

uint8_t bit_vector_is_invalid(bit_vector* vector);
uint32_t bit_vector_1_count(bit_vector* vector, uint32_t from, uint32_t to);
bit_vector* create_bit_vector(uint32_t index);
// bit_vector resize_bit_vector(bit_vector vector, uint32_t index);

uint8_t bit_vector_get(bit_vector* vector, uint32_t index);
void bit_vector_set(bit_vector* vector, uint32_t index, uint8_t value);

void bit_vector_push_last(bit_vector* vector, uint8_t value);
uint8_t bit_vector_pop_last(bit_vector* vector);
void bit_vector_insert(bit_vector* vector, uint32_t index, uint8_t value);
void bit_vector_remove(bit_vector* vector, uint32_t index);

void bit_vector_push_first(bit_vector* vector, uint8_t value);
uint8_t bit_vector_pop_first(bit_vector* vector);
// bit_vector* bit_vector_split(bit_vector* vector, uint32_t index);
// bit_vector* bit_vector_splice(bit_vector* a, bit_vector* b);

void free_bit_vector(bit_vector* vector);

#endif //_BIT_VECTOR_H

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_BIT_VECTOR_C)
    #define _BIT_VECTOR_C

#define BIT_TO_CHUNK(index) (index/8)

uint8_t bit_vector_is_invalid(bit_vector* vector)
{
    return (vector->chunks == NULL && 1 /*vector->last_index != EMPTY_BIT_VECTOR*/) || vector->set_bit_count > vector->last_index + 1;
}

uint8_t _get_set_bit_count(uint8_t vector)
{
    uint8_t count = 0;
    while (vector > 0)
    {
        vector &= (vector - 1);
        count++;
    }
    return count;
}

uint32_t bit_vector_1_count(bit_vector* vector, uint32_t from, uint32_t to)
{
    if(from > to){uint32_t temp = to; to = from; from = temp;}
    if(from > vector->last_index) return 0;
    if(to > vector->last_index) to = vector->last_index;
    uint32_t result = 0;
    uint8_t mask = ~((1<<from%8)-1);

    uint32_t i = from/8;
    for(;i <= to/8; i++)
    {
        if(i != from/8) mask = -1;
        if(i == to/8) mask &= (1<<to%8+1)-1;
        result += _get_set_bit_count(vector->chunks[i] & mask);
    }

    return result;
}

void _realloc_bit_vector(bit_vector* vector, uint32_t index)
{
    //TODO EMPTY_BIT_VECTOR stuff
    if(vector->last_index == index && vector->chunks != NULL) return;

    uint32_t old_size = 1 + BIT_TO_CHUNK(vector->last_index);
    uint32_t new_size = 1 + BIT_TO_CHUNK(index);
    // if(index == EMPTY_BIT_VECTOR) new_size = 1;

    //calculate set bit difference
    uint32_t set_bit_diff = 0;
    if(index < vector->last_index)
    {
        uint32_t current_chunk = new_size - 1;

        uint8_t mask = ~((1<<index%8+1)-1); //upper mask
        if(old_size == new_size) mask &= (1<<vector->last_index%8 + 1) - 1; //overlap with last_index

        set_bit_diff = _get_set_bit_count(vector->chunks[current_chunk] & mask);

        for(current_chunk++; current_chunk <= BIT_TO_CHUNK(vector->last_index); current_chunk++)
            set_bit_diff += _get_set_bit_count(vector->chunks[current_chunk]);
    }

    uint8_t* new_chunks = vector->chunks;

    if(new_chunks == NULL)
    {
        new_chunks = (uint8_t*)calloc(new_size,1);
        if(new_chunks == NULL) return;
    }
    else
    {
        if(new_size != old_size)
        {
            new_chunks = (uint8_t*)realloc(vector->chunks, new_size);
            if(new_chunks == NULL) return;
            //set new bits to 0
            if(new_size > old_size)
            {
                uint32_t current_chunk = old_size-1;
                new_chunks[current_chunk++] &= (1<<vector->last_index%8+1)-1;
                while(current_chunk < new_size) new_chunks[current_chunk++] = 0;
            }
        }
    }

    vector->set_bit_count -= set_bit_diff;
    if(new_chunks != vector->chunks) free(vector->chunks);
    vector->chunks = new_chunks;
    vector->last_index = index;
}

bit_vector* create_bit_vector(uint32_t index)
{
    bit_vector* vector = (bit_vector*)calloc(1, sizeof(bit_vector));

    _realloc_bit_vector(vector, index);

    return vector;
}

//bit_vector resize_bit_vector(bit_vector vector, uint32_t index); //doesnt size memory down, only up

uint8_t _get_bit_chunk(bit_vector* vector, uint32_t index)
{
    uint8_t mask = -1;
    if(BIT_TO_CHUNK(index) == BIT_TO_CHUNK(vector->last_index)) mask = (1<<vector->last_index%8+1)-1;
    return vector->chunks[BIT_TO_CHUNK(index)] & mask;
}

inline uint8_t _get_bit(uint8_t f, uint8_t bit_pos)
{
    return (f & (1<<bit_pos)) >> bit_pos;
}

uint8_t bit_vector_get(bit_vector* vector, uint32_t index)
{
    if(bit_vector_is_invalid(vector) || index > vector->last_index) return 0;

    return _get_bit(_get_bit_chunk(vector, index), index%8);
}

inline uint8_t _set_bit(uint8_t c, uint8_t bit_pos, uint8_t value)
{
    value = value<<bit_pos;
    return (c & ~(1<<bit_pos)) + value;
    // return value ? f | value : f & value; // maybe use xor?
}

void bit_vector_set(bit_vector* vector, uint32_t index, uint8_t value)
{
    if(bit_vector_is_invalid(vector) || index > vector->last_index) return;

    uint8_t old_chunk = _get_bit_chunk(vector, index);
    uint8_t new_chunk = _set_bit(old_chunk, index%8, value!=0);

    if(new_chunk > old_chunk) vector->set_bit_count++;
    if(new_chunk < old_chunk) vector->set_bit_count--;

    vector->chunks[BIT_TO_CHUNK(index)] = new_chunk;
}

void bit_vector_push_last(bit_vector* vector, uint8_t value)
{
    if(bit_vector_is_invalid(vector)) return;

    _realloc_bit_vector(vector, vector->last_index + 1);

    return bit_vector_set(vector, vector->last_index, value);
}

uint8_t bit_vector_pop_last(bit_vector* vector)
{
    uint8_t result = _get_bit(vector->chunks[BIT_TO_CHUNK(vector->last_index)], vector->last_index%8);

    if(result && vector->set_bit_count) vector->set_bit_count--;
    if(vector->last_index) vector->last_index--;

    return result;
}

void bit_vector_insert(bit_vector* vector, uint32_t index, uint8_t value)
{
    if(bit_vector_is_invalid(vector) || index > vector->last_index) return;

    bit_vector_push_last(vector, 0);

    uint32_t current_chunk = BIT_TO_CHUNK(vector->last_index);
    uint32_t first_chunk = BIT_TO_CHUNK(index);
    for(; current_chunk > first_chunk; current_chunk--)
    {
        uint8_t overflow = vector->chunks[current_chunk-1] & 1<<7;
        uint8_t c = vector->chunks[current_chunk];
        vector->chunks[current_chunk] = _set_bit(c<<1, 0, overflow>>7);
    }

    uint8_t lower_mask = 1<<index%8 - 1;
    uint8_t upper_mask = ~(1<<(index+1)%8-1);

    uint8_t c = vector->chunks[current_chunk];
    vector->chunks[current_chunk] = (c&lower_mask) + ((c<<1)&upper_mask);

    bit_vector_set(vector, index, value);
}

void bit_vector_remove(bit_vector* vector, uint32_t index)
{
    if(index > vector->last_index) return;
    if(bit_vector_is_invalid(vector)) return;

    uint8_t underflow = 0;
    uint32_t current_chunk = BIT_TO_CHUNK(vector->last_index);
    uint32_t first_chunk = BIT_TO_CHUNK(index);
    for(; current_chunk > first_chunk; current_chunk--)
    {
        uint8_t c = vector->chunks[current_chunk];
        vector->chunks[current_chunk] = _set_bit(c>>1, 7, underflow);
        underflow = c & 1;
    }

    if(_get_bit(vector->chunks[first_chunk], index%8)) vector->set_bit_count--;

    uint8_t bit_mask = 1<<index%8;
    uint8_t lower_mask = bit_mask - 1;
    uint8_t upper_mask = ~lower_mask;

    uint8_t c = vector->chunks[current_chunk];
    vector->chunks[current_chunk] = _set_bit((c&lower_mask) + (c>>1&upper_mask), 7, underflow);

    _realloc_bit_vector(vector, vector->last_index - 1);
}

void bit_vector_push_first(bit_vector* vector, uint8_t value)
{
    bit_vector_insert(vector, 0, value);
}

uint8_t bit_vector_pop_first(bit_vector* vector)
{
    uint8_t result = bit_vector_get(vector, 0);
    bit_vector_remove(vector, 0);

    return result;
}

// bit_vector bit_vector_split(bit_vector* vector, uint32_t index)
// {
//     //TODO implement
// }

// bit_vector bit_vector_splice(bit_vector a, bit_vector b)
// {
//     //TODO implement
// }

void free_bit_vector(bit_vector* vector)
{
    if(vector == NULL) return;
    if(vector->chunks != NULL) free(vector->chunks);
    free(vector);
}

#endif //_BIT_VECTOR_C

//TODO implement EMPTY_BIT_VECTOR
//TODO inspect concern with API using 4 byte chunks inside functions (instead of 1 byte)
//TODO consider ways of avoiding all the validations