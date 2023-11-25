#include <stdlib.h>
#include "crossplatform_app/script_helper.c"
#include "app_configuration.c"

//-------------------------
//Build debug by default, unless release is specified (-D RELEASE)
#define DEBUG_FLAGS " -g"
#ifdef RELEASE
#undef DEBUG_FLAGS
#define DEBUG_FLAGS ""
#endif

#define WINDOWS_FLAGS ""
#ifdef _WIN32
#undef WINDOWS_FLAGS
#define WINDOWS_FLAGS " -Wl,-subsystem,windows"
#endif

#if defined __APPLE__ || __linux__
#endif

//-------------------------
#define FLAGS DEBUG_FLAGS " -w" WINDOWS_FLAGS

#define LIBRARY_PATHS " -L./crossplatform_app/backend/SDL2/lib/x86"
#define LIBRARY_NAMES " -lSDL2"
#define LIBRARIES LIBRARY_PATHS LIBRARY_NAMES

int main()
{
    printf("Attempting build...\n");

    //clear build folder
    fs_delete("build/", NON_RECURSIVE);

    //check for pre-compiled objects and update if necessary


    //build actual program
    char command[256];
    sprintf(command, COMPILER " main.c" LIBRARIES FLAGS " -o build/"EXECUTABLE);
    system(command);

    //copy necessary libraries
    #ifdef _WIN32
    fs_copy("crossplatform_app/backend/SDL2/lib/x86/SDL2.dll", "build/", NON_RECURSIVE);
    #endif

    #if defined __APPLE__ || __linux__
    fs_copy("crossplatform_app/backend/SDL2/lib/x86/libSDL2.a", "build/", NON_RECURSIVE);
    #endif

    printf("Done.\n");
}