#include "app_configuration.c"
#include "crossplatform_app/api.c"

int main()
{
    setup_app();

    game_loop();

    close_app();
}