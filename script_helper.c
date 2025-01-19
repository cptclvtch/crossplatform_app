#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#define INTERPRETER "tcc"
#define COMPILER "tcc"
#define DEBUGGER "gdb"

#ifdef _WIN32
#define EXECUTABLE APP_NAME".exe"
#else
#define EXECUTABLE APP_NAME
#endif

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

void sterilize_path(char* path)
{
    replace_characters(path, '/', '\\');
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
    sterilize_path(buffer);
    #else
    getcwd(buffer, size);
    #endif
}

#define NON_RECURSIVE 0
#define RECURSIVE 1
void fs_copy(char* origin, char* destination, uint8_t recursive)
{
    char command[256];
    sterilize_path(origin);
    sterilize_path(destination);

    #ifdef _WIN32
    sprintf(command, "xcopy /q /i /d %s %s", origin, destination);
    if(recursive) sprintf(command, "%s /e", command);
    #endif

    #if defined __APPLE__ || __linux__
    sprintf(command, "cp -q -u %s %s", origin, destination);
    if(recursive)sprintf(command, "%s -r", command);
    #endif

    system(command);
}

void fs_delete(char* path, uint8_t recursive)
{
    char command[256];
    sterilize_path(path);

    #ifdef _WIN32
    sprintf(command, "del /q %s", path);
    if(recursive) sprintf(command, "%s /s", command);
    #endif

    #if defined __APPLE__ || __linux__
    sprintf(command, "rm %s", path);
    if(recursive) sprintf(command, "%s -r", command);
    #endif

    system(command);
}