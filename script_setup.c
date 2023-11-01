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
}