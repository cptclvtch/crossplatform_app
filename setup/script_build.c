#include <stdlib.h>

#define EXECUTABLE "main.exe"
#define PATH "windows"

#ifndef WIN32
#undef EXECUTABLE
#undef PATH
#endif

#if defined __APPLE__ || __linux__
#define EXECUTABLE "main"
#define PATH "osx"
#endif

int main()
{
    system("tcc main.c -L.\crossplatform_app\backend\SDL2\lib\x86 -lSDL2 -w -Wl,-subsystem,windows -o build/windows/"EXECUTABLE);
}