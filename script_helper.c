#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#define COMPILER "tcc"
#define DEBUGGER "gdb"

void replace_characters(char* buffer, char from, char to)
{
    unsigned int i = 0;
    while(buffer[i])
    {
        if(buffer[i] == from)
            buffer[i] = to;
        i++;
    }
}

void set_cwd(char* path)
{
    #ifdef _WIN32
    SetCurrentDirectory(path);
    #else
    chdir(path);
    #endif
}

void get_cwd(char* buffer, size_t size)
{
    #ifdef _WIN32
    GetCurrentDirectory(size, buffer);
    replace_characters(buffer, '/', '\\');
    #else
    getcwd(buffer, size);
    #endif
}

#define NON_RECURSIVE 0
#define RECURSIVE 1
void fs_copy(char* origin, char* destination, uint8_t recursive)
{
    char command[256];

    #ifdef _WIN32
    replace_characters(origin, '/', '\\');
    replace_characters(destination, '/', '\\');
    sprintf(command, "xcopy /q /i %s %s", origin, destination);
    if(recursive) sprintf(command, "%s /e", command);
    #endif

    #if defined __APPLE__ || __linux__
    sprintf(command, "cp -q %s %s", origin, destination);
    if(recursive)sprintf(command, "%s -r", command);
    #endif

    system(command);
}

void fs_delete(char* path, uint8_t recursive)
{
    char command[256];

    #ifdef _WIN32
    replace_characters(path, '/', '\\');
    sprintf(command, "del /q %s", path);
    if(recursive) sprintf(command, "%s /s", command);
    #endif

    #if defined __APPLE__ || __linux__
    sprintf(command, "rm %s", path);
    if(recursive) sprintf(command, "%s -r", command);
    #endif

    system(command);
}