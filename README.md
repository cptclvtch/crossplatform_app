# What is this?
 This is a C template meant to be used as a starting point for creating crossplatform projects.
 It comes bundled with SDL2 and Nuklear GUI, so feel free to use any of the functions they provide!

## Supported Platforms:
- Windows

## How to setup a project
- Copy this repository into your project folder.

NOTE: If you are on Windows, make sure you have the paths to ```tools/tcc/win32``` and ```tools/gdb``` in your PATH environment variable.
NOTE: If you are on Linux, make sure you install ```tools/tcc/linux/tcc_0.9.27-5_amd64.deb```.

- Open the command line and navigate to your project folder.

- Set up the project by running the following command:
```tcc crossplatform_app/script_setup.c -run```

This completes the setup process!

For compilation instructions, please read the following section.

## How to use the API

- Start by modifying the ```main.c``` file in your project folder.

    The example ```main.c``` code:
    ```
    #include "app_configuration.c"
    #include "crossplatform_app/api.c"

    int main()
    {
        setup();

        game_loop();

        close();
    }
    ```
- Build and run your app using the provided scripts: ```script_build.c``` and ```script_run.c```.

## Licensing
This template comes bundled with the following technologies and their licenses:
- SDL2 (zlib license)
- Nuklear GUI (Public Domain)
- TCC (GNU Lesser General Public License)