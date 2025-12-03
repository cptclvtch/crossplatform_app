# What is this?
 This is a C project template meant to be used as a starting point for creating crossplatform projects.
 It comes bundled with SDL2 and Nuklear GUI, so feel free to use any of the functions they provide!

## Features:
- Collision API
- Physics API
- Geometric Algebra API (vector and rotor math)
- Data Structures
    - With Template Support: Linked list, Binary tree
    - No Template Support: Bit vector
- Frame Timing API
- simple Asset Manager API

## Supported Platforms:
- Windows

## How to setup a project
- Copy this repository into your project folder.

NOTE: If you are on Windows, make sure you have the paths to ```tools/tcc/win32``` and ```tools/gdb``` in your PATH environment variable.
NOTE: If you are on Linux, make sure you install ```tools/tcc/linux/tcc_0.9.27-5_amd64.deb```.

- Open the command line and navigate to your project folder.

- Set up the project by running the following command:
```tcc crossplatform_app/script_setup.c -run```

## Alternative project setup
- You can also setup a project without copying this repository!

- Simply go to your crossplatform_app folder and run the following command while specifying your project folder path:
```tcc script_setup.c -run -DPROJECT_FOLDER=../your/project/folder```

- Don't forget to update CROSSPLATFORM_APP_FOLDER in app_configuration.c to the path of crossplatform_app relative to your project!

This completes the setup process!

For compilation instructions, please read the following section.

## How to use the API

- Start by modifying the ```main.c``` file in your project folder.

    The example ```main.c``` code:
    ```
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
    ```
- Build and run your app using the provided scripts: ```script_build.c``` and ```script_run.c```.

## Licensing
This template comes bundled with the following technologies and their licenses:
- template code (MIT license)
- SDL2 (zlib license)
- Nuklear GUI (Public Domain)
- TCC (GNU Lesser General Public License)
- fptc (BSD license)