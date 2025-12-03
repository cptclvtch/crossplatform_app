#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "apis/data_structures/linked_list/api.c"

#define INTERPRETER "tcc"
#ifndef COMPILER
    #define COMPILER "tcc"
#endif
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

void add_value_to_environment_variable(char* val, char* var)
{
    //check if variable exists
        //if not, create it

    //check if value exists in variable
        //if not, add it
        //if it does, return
    
    char* str = getenv(var);

    if(str == NULL)
    {
        // printf();
    }

    char* token = strtok(str, ";");
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, ";");
    }
}

ll_node* get_list_of_files(char* path, char* extension);
ll_node* get_list_of_files(char* path, char* extension)
{
    ll_node* list = NULL;

    //search for files
    #ifdef _WIN32
        // https://learn.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory
        WIN32_FIND_DATA ffd;
        HANDLE hFind;
        char query[MAX_PATH];

        //do recursive search
        sprintf(query, "%s/*", path);
        hFind = INVALID_HANDLE_VALUE;
        hFind = FindFirstFile(query, &ffd);
        if (hFind != INVALID_HANDLE_VALUE)
            do
            {
                if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && ffd.cFileName[0] != '.')
                {
                    sprintf(query, "%s/%s", path, ffd.cFileName);
                    ll_node* recursive_return = get_list_of_files(query, extension);
                    if(recursive_return)
                    {
                        //prepend to current list
                        while(recursive_return->NEXT) recursive_return = recursive_return->NEXT;
                        list->PREV = recursive_return;
                        recursive_return->NEXT = list;
                        while(list->PREV) list = list->PREV;
                    }
                }
            }
            while(FindNextFile(hFind, &ffd) != 0);
        FindClose(hFind);

        // search current folder
        sprintf(query, "%s/*%s", path, extension);

        hFind = INVALID_HANDLE_VALUE;
        hFind = FindFirstFile(query, &ffd);
        if (hFind != INVALID_HANDLE_VALUE)
            do
            {
                if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    //TODO fix first item not being added to list
                    // printf("%s\n", ffd.cFileName);
                    char** tuple = (char**)malloc(2*sizeof(char*));
                    tuple[0] = (char*)malloc(MAX_PATH);
                    strcpy(tuple[0], path);
                    tuple[1] = (char*)malloc(64);
                    strcpy(tuple[1], ffd.cFileName);
                    list = ll_add_data(list, PREV, tuple);
                }
            }
            while(FindNextFile(hFind, &ffd) != 0);
        FindClose(hFind);
    #endif
    #if defined __APPLE__ || __linux__
        //TODO
        #error Please implement me
        //ls
    #endif

    return list;
}