# What is this?
 This is a template meant to be used as a starting point for creating crossplatform projects.

## How to setup a project
- Clone this template into your project folder using ```git clone --recurse-submodules https://github.com/cptclvtch/crossplatform_app```.
- Add the 'tcc' executable to your PATH environment variable. You can find the executable in ```tools/tcc```.
- Restart your command line.

NOTE: Now you can run all the included scripts using a ```tcc YOUR_script.c -run``` command.

- Run ```setup_script.c``` located in the ```crossplatform_app``` folder.

This completes the setup process!

## How to use the API

- Start building your app by modifying the ```main.c``` file in your project folder.
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
- Build and run your app using the provided scripts: ```build_script.c``` and ```run_script.c```.

NOTE: DO NOT ATTEMPT TO CIRCUMVENT THE CLONING PROCESS