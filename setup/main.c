#include "app_configuration.c"
#include STRINGIFY(CROSSPLATFORM_APP_FOLDER/api.c)

void process_input()
{}

void update_app()
{}

void render_output()
{}

int main()
{
    setup_app();

    app_loop();

    close_app();
}