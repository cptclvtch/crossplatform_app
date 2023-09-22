#include <stdlib.h>

#define WINDOWS_FLAGS ""

#ifdef WIN32
#define EXECUTABLE "main.exe"
#define PATH "windows"
#undef WINDOWS_FLAGS
#define WINDOWS_FLAGS "-subsystem,windows"
#endif

#if defined __APPLE__ || __linux__
#define EXECUTABLE "main"
#define PATH "osx"
#endif

int main()
{
    system("tcc main.c -L./crossplatform_app/backend/SDL2/lib/x86 -lSDL2 -w -Wl,"WINDOWS_FLAGS" -o build/"PATH"/"EXECUTABLE);
}