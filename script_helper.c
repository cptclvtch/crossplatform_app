#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void set_cwd(char* path)
{
    #ifdef WIN32
    SetCurrentDirectory(path);
    #else
    chdir(path);
    #endif
}

void get_cwd(char* buffer, size_t size)
{
    #ifdef WIN32
    GetCurrentDirectory(size, buffer);
    #else
    getcwd(buffer, size);
    #endif
}