#include <stdio.h>
#include <stdint.h>

uint8_t array[8];
uint8_t count = 0;

uint8_t* return_int()
{
    array[count] = 1;
    return &(array[count]);// =
        //   1);
}

int main()
{
    printf("%p[0] : %p\n", array, &(array[0]));

    uint8_t* outter_int = NULL;

    {
        uint8_t* my_int = return_int();
        outter_int = my_int;

        printf("\t%p[0] : %p\n", array, my_int);
        printf("\t%p[0] : %p\n", array, my_int);
    }
    printf("%p[0] : %p\n", array, outter_int);
    printf("%p[0] : %p\n", array, outter_int);
}