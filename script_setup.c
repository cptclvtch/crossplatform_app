//#!/tools/tcc/win32
#include <stdlib.h>
#include <stdio.h>

#include "script_helper.c"

int main()
{
    //git setup
    char cwd[512];
    char formatted_command[512];
    get_cwd(cwd, sizeof(cwd));    
    sprintf(formatted_command, "git config --global --add safe.directory %s", cwd);
    system(formatted_command);
    printf("Making the current directory safe for git.\n %s\n\n",formatted_command);
    
    system("git submodule init");
    system("tcc setup/script_update_api.c -run");
}