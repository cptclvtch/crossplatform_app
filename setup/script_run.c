#include <stdlib.h>

#ifdef WIN32
#define EXECUTABLE "main.exe"
#define PATH "windows"
#endif

#ifdef __APPLE__
#define EXECUTABLE "main"
#define PATH "osx"
#endif

#ifdef __linux__
#define EXECUTABLE "main"
#define PATH "linux"
#endif

int main()
{
    #define START ""
    #ifdef WIN32
        #undef START
        #define START "start "
    #endif

    system(START"./build/"PATH"/"EXECUTABLE);
}