#define APP_NAME "app"


/*----------------------MACRO-POST-PROCESSING-------------------------*/
#ifdef _WIN32
#define EXECUTABLE APP_NAME".exe"
#else
#define EXECUTABLE APP_NAME
#endif