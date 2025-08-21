#include "api.c"
#define INCLUDE_IMPLEMENTATION
#include "api.c"

#include <stdio.h>

int main()
{
    frame_timing_setup(60);

    uint8_t i = 0;
    for(; i < MAX_VSYNC_MULTIPLES; i++)
    {
        printf("Frequency %u: %u nanoseconds (%u fps)\n",i, vsync_frametime_snaps[i], 1000000/vsync_frametime_snaps[i]);
    }

    
}