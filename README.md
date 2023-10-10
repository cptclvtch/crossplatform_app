# What is this?
 This is a C template meant to be used as a starting point for creating crossplatform projects.
 It comes bundled with SDL2 and Nuklear GUI, so feel free to use any of the functions they provide!

## Supported Platforms:
- Windows

## How to setup a project
- Clone this repository into your project folder.

- Add the 'tcc' executable folder to your PATH environment variable. TCC (Tiny C Compiler) is used for all the compilation, scripts and standard-keeping on this project. You can find the executable in ```tools/tcc/win32``` for Windows and ```tools/tcc/linux``` for OSX/Linux.
- Restart your command line if you had one open.

NOTE: You can now run all the included scripts using a ```tcc script_NAME.c -run``` command.

- Run ```script_setup.c``` located in the ```crossplatform_app``` folder.

This completes the setup process!

For compilation instructions, please read the following section.

## How to use the API

- Start by modifying the ```main.c``` file in your project folder.

    The example ```main.c``` code:
    ```
    #include "crossplatform_app/api.c"

    int main()
    {
        setup();

        loop();

        close();
    }
    ```
- Build and run your app using the provided scripts: ```script_build.c``` and ```script_run.c```.

- If you would like to update the API to the newest version on Github, run the ```script_update_api.c``` script.

## Licensing
This template comes bundled with the following technologies and their licenses:
- SDL2 (zlib license)
- Nuklear GUI (Public Domain)
- TCC (GNU Lesser General Public License)