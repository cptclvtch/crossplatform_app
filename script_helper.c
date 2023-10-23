#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void switch_slashes(char* buffer, size_t size)
{
    unsigned int i = 0;
    while(i < size)
    {
        if(buffer[i] == '\\')
            buffer[i] = '/';
        i++;
    }
}

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
    switch_slashes(buffer, size);
    #else
    getcwd(buffer, size);
    #endif
}

void fs_delete(const char* path)
{
    char command[256];
    #ifdef WIN32
    sprintf(command,"del %s", path);
    system(command);
    #else

    #endif
}