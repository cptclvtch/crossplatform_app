#include "../../unit_testing.c"
#include "api.c"
#define INCLUDE_IMPLEMENTATION
#include "api.c"

void print_bit_vector(bit_vector* v)
{
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
    bit_vector *b_a, *b_b;
    uint8_t a,b;
    #define TITLE "internals"
    POST_TITLE

    #define SUBTITLE "bit_vector_is_invalid - Valid"
    b_a = &(bit_vector){0};
    b_a->chunks = (uint8_t*)calloc(1,1);
    VERIFY_SINGLE_VALUE(bit_vector_is_invalid(b_a), ==, 0);
    COLLECT_FINDINGS
    free(b_a->chunks);

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_is_invalid - NULL chunks"
    b_a = &(bit_vector){0};
    VERIFY_SINGLE_VALUE(bit_vector_is_invalid(b_a), ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "bit_vector_is_invalid - set_bit_count greater than last_index"
    b_a = &(bit_vector){0};
    b_a->set_bit_count = 1;
    VERIFY_SINGLE_VALUE(bit_vector_is_invalid(b_a), ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "_get_set_bit_count - 0"
    a = 0;
    VERIFY_SINGLE_VALUE(_get_set_bit_count(a), ==, 0);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "_get_set_bit_count - 1"
    a = 1;
    VERIFY_SINGLE_VALUE(_get_set_bit_count(a), ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "_get_set_bit_count - 2"
    a = 2;
    VERIFY_SINGLE_VALUE(_get_set_bit_count(a), ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "_get_set_bit_count - 0xff"
    a = -1;
    VERIFY_SINGLE_VALUE(_get_set_bit_count(a), ==, 8);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "_realloc_bit_vector - NULL chunks"
    b_a = &(bit_vector){0};
    _realloc_bit_vector(b_a, 1);
    VERIFY_SINGLE_VALUE(b_a->chunks, !=, NULL);
    VERIFY_SINGLE_VALUE(b_a->last_index, ==, 1);
    COLLECT_FINDINGS

    //TODO EMPTY_BIT_VECTOR

    #undef SUBTITLE
    #define SUBTITLE "_realloc_bit_vector - same indices"
    uint8_t* old_memory = b_a->chunks;
    _realloc_bit_vector(b_a, 1);
    VERIFY_SINGLE_VALUE(b_a->chunks, ==, old_memory);
    VERIFY_SINGLE_VALUE(b_a->last_index, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "_realloc_bit_vector - from 1 to 0"
    b_a->chunks[0] = 3;
    b_a->set_bit_count = _get_set_bit_count(3);
    _realloc_bit_vector(b_a, 0);
    VERIFY_SINGLE_VALUE(b_a->chunks, ==, old_memory);
    VERIFY_SINGLE_VALUE(b_a->chunks, !=, NULL);
    VERIFY_SINGLE_VALUE(b_a->last_index, ==, 0);
    VERIFY_SINGLE_VALUE(b_a->set_bit_count, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "_realloc_bit_vector - from 0 to 9"
    old_memory = b_a->chunks;
    _realloc_bit_vector(b_a, 9);
    VERIFY_SINGLE_VALUE(b_a->chunks, !=, old_memory);
    VERIFY_SINGLE_VALUE(b_a->last_index, ==, 9);
    VERIFY_SINGLE_VALUE(b_a->set_bit_count, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "_set_bit - first bit"
    a = _set_bit(0, 0, 1);
    VERIFY_SINGLE_VALUE(a, ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "_set_bit - last bit"
    a = _set_bit(a, 7, 1);
    VERIFY_SINGLE_VALUE(a, ==, 129);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "_set_bit - set first to 0"
    a = _set_bit(a, 0, 0);
    VERIFY_SINGLE_VALUE(a, ==, 128);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "_get_bit"
    VERIFY_SINGLE_VALUE(_get_bit(a, 7), ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "_get_bit_chunk - b_a[0]"
    VERIFY_SINGLE_VALUE(_get_bit_chunk(b_a, 0), ==, 1);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "creation and deletion"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "create_bit_vector"
    b_a = create_bit_vector(9);
    VERIFY_SINGLE_VALUE(b_a->last_index, ==, 9);
    VERIFY_SINGLE_VALUE(b_a->set_bit_count, ==, 0);
    COLLECT_FINDINGS
    free(b_a->chunks);

    #undef SUBTITLE
    #define SUBTITLE "create_bit_vector - 0 last index"
    b_a = create_bit_vector(0);
    VERIFY_SINGLE_VALUE(bit_vector_is_invalid(b_a), ==, 0);
    VERIFY_SINGLE_VALUE(b_a->chunks, !=, NULL);
    VERIFY_SINGLE_VALUE(b_a->last_index, ==, 0);
    VERIFY_SINGLE_VALUE(b_a->set_bit_count, ==, 0);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "freeing vector - shouldn't crash"
    free_bit_vector(b_a);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "getting and setting"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_get - Best Case Scenario"
    b_a->chunks[0] = 3;
    b_a->set_bit_count = 2;
    b_a->last_index = 1;
    VERIFY_SINGLE_VALUE(bit_vector_get(b_a, 0), ==, 1);
    VERIFY_SINGLE_VALUE(bit_vector_get(b_a, 1), ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_get - Out of range"
    VERIFY_SINGLE_VALUE(bit_vector_get(b_a, 2), ==, 0);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "bit_vector_get - Way out of range"
    VERIFY_SINGLE_VALUE(bit_vector_get(b_a, 9), ==, 0);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_set - Best Case Scenario"
    bit_vector_set(b_a, 0, 1);
    VERIFY_SINGLE_VALUE(bit_vector_get(b_a, 0), ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_set - set 0"
    bit_vector_set(b_a, 0, 0);
    VERIFY_SINGLE_VALUE(bit_vector_get(b_a, 0), ==, 0);
    VERIFY_SINGLE_VALUE(b_a->set_bit_count, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_set - Out of range"
    bit_vector_set(b_a, 0, 9);
    VERIFY_SINGLE_VALUE(bit_vector_is_invalid(b_a), ==, 0);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "push and pop"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_push_last - Best Case Scenario"
    b_b = create_bit_vector(0);
    bit_vector_push_last(b_b, 1);
    VERIFY_SINGLE_VALUE(bit_vector_get(b_b, 1), ==, 1);
    VERIFY_SINGLE_VALUE(b_b->set_bit_count, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_push_last - push 0"
    bit_vector_push_last(b_b, 0);
    VERIFY_SINGLE_VALUE(bit_vector_get(b_b, 2), ==, 0);
    VERIFY_SINGLE_VALUE(b_b->set_bit_count, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_pop_last - Best Case Scenario"
    VERIFY_SINGLE_VALUE(bit_vector_pop_last(b_b), == , 0);
    VERIFY_SINGLE_VALUE(b_b->set_bit_count, ==, 1);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_pop_last - No More Left"
    bit_vector_pop_last(b_b);
    bit_vector_pop_last(b_b);
    VERIFY_SINGLE_VALUE(b_b->set_bit_count, ==, 0);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, 0);
    bit_vector_pop_last(b_b);
    VERIFY_SINGLE_VALUE(b_b->set_bit_count, ==, 0);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, 0);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "inserting, removing & bit count"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "inserting - Best Case Scenario"
    free_bit_vector(b_b);
    b_b = create_bit_vector(5);
    bit_vector_push_last(b_b, 1);
    bit_vector_push_last(b_b, 1);
    bit_vector_insert(b_b, 1, 1);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, 8);
    VERIFY_SINGLE_VALUE(bit_vector_get(b_b, 1), ==, 1);
    VERIFY_SINGLE_VALUE(bit_vector_get(b_b, 8), ==, 1);
    VERIFY_SINGLE_VALUE(b_b->chunks[0], ==, 130);
    VERIFY_SINGLE_VALUE(b_b->chunks[1], ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "inserting - Out of range"
    bit_vector_insert(b_b, 666, 1);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, 8);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "bit_vector_1_count - Best Case Scenario"
    VERIFY_SINGLE_VALUE(bit_vector_1_count(b_b, 0, b_b->last_index), ==, b_b->set_bit_count);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "bit_vector_1_count - middle (case A)"
    VERIFY_SINGLE_VALUE(bit_vector_1_count(b_b, 2, b_b->last_index), ==, 2);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bit_vector_1_count - middle (case B)"
    VERIFY_SINGLE_VALUE(bit_vector_1_count(b_b, 2, b_b->last_index-1), ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "bit_vector_1_count - middle (case C)"
    VERIFY_SINGLE_VALUE(bit_vector_1_count(b_b, 2, 3), ==, 0);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "removing - from the middle"
    uint8_t last = b_b->last_index;
    bit_vector_remove(b_b, 1);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, last-1);
    VERIFY_SINGLE_VALUE(_get_bit_chunk(b_b, 0), ==, 64 + 128);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "removing - last"
    bit_vector_remove(b_b, last-1);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, last-2);
    VERIFY_SINGLE_VALUE(_get_bit_chunk(b_b,0), ==, 64);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "removing - Out of range"
    bit_vector_remove(b_b, 666);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, last-2);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "removing - first"
    bit_vector_remove(b_b, 0);
    VERIFY_SINGLE_VALUE(b_b->last_index, ==, last-3);
    VERIFY_SINGLE_VALUE(_get_bit_chunk(b_b,0), ==, 32);
    COLLECT_FINDINGS

    DEBRIEF
}