#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void current_working_directory(char* buffer, size_t size)
{
    #ifdef WIN32
    GetCurrentDirectory(size, buffer);
    #else
    getcwd(buffer, size);
    #endif
}

int main()
{
    //git setup
    char cwd[512];
    char formatted_command[512];
    current_working_directory(cwd, sizeof(cwd));    
    sprintf(formatted_command, "git config --global --add safe.directory %s", cwd);
    system(formatted_command);
    // system("git submodule --init");

    //Recursively copy the setup folder
    #ifdef WIN32
    system("xcopy /e /h /i .\\setup\\ ..\\");
    #endif

    #if defined __APPLE__ || __linux__
    system("cp -R ./setup/ ../");
    #endif
}