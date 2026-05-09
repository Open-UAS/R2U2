# Project Overview
Implementing R2U2 on the OpenUAS

## ESP-32
Working on an esp-32 Devkit v1. Connected to Pixhawk using I2C protocol.

## ESP IDF
I'm using the Espressif ESP IDF framework because R2U2 only works in C

# Project Workflow
## WSL
R2U2 does not function on Windows, so use the Windows Subsystem for Linux to run R2U2 locally.

### Accessing a Microcontroller from WSL
Guide: <https://learn.microsoft.com/en-us/windows/wsl/connect-usb>
1. Download the [udbipd-win project](https://github.com/dorssel/usbipd-win/releases) from GitHub. Run the downloaded .msi file to start the installer.
2. In command window/Powershell (as an administrator), run "usbipd list" to see the list of usb ports currently in use. Run this command twice, once without the controller plugged in and once without.
3. Bind the port that only appears with the controller plugged in using "usbipd bind --busid \<busid\>" ex: "usbipd bind --busid 4-4".
4. In a text editor, write the command "usbipd attach --wsl --busid \<busid\>" and save it as a .bat file.
5. Whenever you plug in the controller, execute the .bat file.
6. To see the available usb devices on Ubuntu, run the "lsusb" command in the Linux terminal. It will probably be "/dev/ttyUSB0".

## Espressif
Guide with CLI build/flash commands: <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-start-project.html#activate-using-eim-cli>
- Command to initiate esp idf in terminal:source "/home/\<usr\>/.espressif/tools/activate_idf_v6.0.1.sh"
    - must be run every time terminal is reset
- to build repository, run "idf.py -p /dev/ttyUSB0 build"
- to flash AND build, run "idf.py -p /dev/ttyUSB0 flash"
- to monitor while running, run "idf.py -p /dev/ttyUSB0 monitor"

### Creating a New Esp Idf Project
- for a new project, instead of running menuconfig as a vscode command, run "idf.py menuconfig" in the command terminal.
- if \<project\>/.vscode/ does not have a file called "c_cpp_properties.json", use the vscode command "ESP-IDF: Add VS Code Configuration Folder"
- follow this guide to add an external library: <https://developer.espressif.com/blog/2025/10/porting-external-library-as-component/>