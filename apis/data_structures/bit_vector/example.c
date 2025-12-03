#include <stdio.h>
#include <stdint.h>

#define INCLUDE_IMPLEMENTATION
#include "api.c"

void print_bit_vector(bit_vector* v)
{
    if(v->last_index == -1) return;
    printf("\n");
    printf("li: %u, sbc: %u ::", v->last_index, v->set_bit_count);

    uint32_t i = 0;
    for(; i <= v->last_index; i++)
    {
        if(i%8 == 0) printf(" ");
        printf("%c", v->chunks[BIT_TO_CHUNK(i)] & 1<<i%8 ? '1' : '0');
    }
    printf("\n");
}

int main()
{
    bit_vector* my_bits = create_bit_vector(0);

    bit_vector_push_first(my_bits, 1);
    bit_vector_push_last(my_bits, 200); //same as 1

    bit_vector_insert(my_bits, 1, 1);
    print_bit_vector(my_bits);

    bit_vector_1_count(my_bits, 0, my_bits->last_index);

    uint8_t bit = bit_vector_pop_last(my_bits);
    print_bit_vector(my_bits);

    bit_vector_remove(my_bits, 0);
    print_bit_vector(my_bits);

    free_bit_vector(my_bits);
}