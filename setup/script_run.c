#include <stdlib.h>
#include "app_configuration.c"

int main()
{
    #define START ""
    #ifdef _WIN32
        #undef START
        #define START "start"
    #endif

    system(START" ./build/" EXECUTABLE);
}