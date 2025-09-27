#include <stdlib.h>
#include "app_configuration.c"
#include "../../../script_helper.c"

//-------------------------
//Build debug by default, unless release is specified (-D RELEASE)
#ifdef RELEASE
#define DEBUG_FLAGS ""
#else
#define DEBUG_FLAGS " -g"
#endif

#define WINDOWS_FLAGS ""
#ifdef _WIN32
#undef WINDOWS_FLAGS
#ifndef RELEASE
#define WINDOWS_FLAGS " -Wl,-subsystem,console"
#else
#define WINDOWS_FLAGS " -Wl,-subsystem,gui"
#endif
#endif

#ifdef RELEASE
#define RELEASE_FLAG "-D RELEASE"
#else
#define RELEASE_FLAG ""
#endif

#if defined __APPLE__ || __linux__
#endif

//-------------------------
#define FLAGS DEBUG_FLAGS " -w" WINDOWS_FLAGS " " RELEASE_FLAG

#define LIBRARY_PATHS " -L../../../backend/SDL2/lib/x86"
#define LIBRARY_NAMES " -lSDL2"
#define LIBRARIES LIBRARY_PATHS LIBRARY_NAMES

//-------------------------
char* sources[256];
#define SOURCE_FOLDER(i) sources[2*i]
#define SOURCE_FILE(i) sources[2*i+1]
uint8_t number_of_sources = 0;
void add_source(char* folder, char* file)
{
    SOURCE_FOLDER(number_of_sources) = folder;
    SOURCE_FILE(number_of_sources) = file;
    number_of_sources++;
}

void compile_sources()
{
    char command[256];
    //folder - source file pairs
    uint8_t i = 0;
    for(;i < number_of_sources;i++)
    {
            get_cwd(command, 256);
            printf("Current directory is: %s", command);
            printf("Compiling %s... ", SOURCE_FOLDER(i));
        set_cwd(SOURCE_FOLDER(i));
            get_cwd(command, 256);
            printf("Now moving into %s, and compiling %s", command, SOURCE_FILE(i));
        sprintf(command, COMPILER" -c %s/%s.c", SOURCE_FOLDER(i), SOURCE_FILE(i));
        system(command);
        printf("Done.\n");
        set_cwd("..");
            get_cwd(command, 256);
            printf("Current directory is: %s", command);
    }
}

int main()
{
    char command[256];
    printf("Attempting build...\n");

    //check for pre-compiled objects and update if necessary
    // add_source("crossplatform_app","api");
    
    char source_paths[2048] = "";
    uint8_t i = 0;
    for(;i < number_of_sources; i++)
        sprintf(source_paths, "%s %s/%s.c", source_paths, SOURCE_FOLDER(i), SOURCE_FILE(i));

    //build actual program
    char executable_name[256] = EXECUTABLE;
    replace_characters(executable_name, ' ', '_');
    sprintf(command, COMPILER " main.c %s" LIBRARIES FLAGS " -o build/%s", source_paths, executable_name);
    printf("%s\n", command);
    if(system(command)) return;

    //copy necessary libraries
    printf("\nCopying libraries... \n");
    printf("-----------------\n");
    #ifdef _WIN32
    fs_copy("../../../backend/SDL2/lib/x86/SDL2.dll", "build/", NON_RECURSIVE);
    #endif

    #if defined __APPLE__ || __linux__
    fs_copy("../../../backend/SDL2/lib/x86/libSDL2.a", "build/", NON_RECURSIVE);
    #endif

    //copy assets
    printf("\nCopying assets... \n");
    printf("-----------------\n");
    #ifdef _WIN32
    #endif

    #if defined __APPLE__ || __linux__
    #endif

    printf("Done.\n");

    #define START ""
    #ifdef _WIN32
        #undef START
        #define START "start"
    #endif

    #ifndef RELEASE
    #undef START
    #define START "gdb -q"
    #endif
    
    set_cwd("./build");

    replace_characters(executable_name, ' ', '_');
    
    sprintf(command, START " %s", executable_name);
    system(command);

    set_cwd("..");
}