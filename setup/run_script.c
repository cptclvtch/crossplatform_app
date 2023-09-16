#include <stdlib.c>

#define EXECUTABLE "main.exe"
#define PATH "windows"

#ifndef WIN32
#undef EXECUTABLE
#undef PATH
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
    system("cd ./build/" PATH);
    system(EXECUTABLE);
    system("cd ../..");
}