#include <stdlib.h>

int main()
{
    //Recursively copy the setup folder
    
    #ifdef WIN32
    system("xcopy ./setup ../ /E/H");
    #endif

    #if defined __APPLE__ || __linux__
    system("cp -R ./setup/ ../");
    #endif
}