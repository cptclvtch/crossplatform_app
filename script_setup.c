//#!/tools/tcc/win32
#include "script_helper.c"

int main()
{
    //Recursively copy the setup folder
    #ifdef WIN32
    system("xcopy /e /i .\\setup\\ ..\\");
    #endif

    #if defined __APPLE__ || __linux__
    system("cp -R ./setup/* ../");
    #endif

    //git setup
    char cwd[512];
    char formatted_command[512];
    get_cwd(cwd, sizeof(cwd));    
    sprintf(formatted_command, "git config --global --add safe.directory %s", cwd);
    printf("Making the current directory safe for git.\n %s\n\n",formatted_command);
    system(formatted_command);
    
    system("git submodule init");
    system("tcc ../script_update_api.c -run");
}