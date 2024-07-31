# ModuleErrorLogger
This project consists of a server and client system using UDP socket programming on a Raspberry Pi 4, leveraging a Linux build system from the Yocto Project. The project includes three main modules: server, application, and display. Each module is implemented using C++ and integrated into the Yocto Project using bitbake, meta-layers, and recipes.

#Project Overview
1. Server Module
2. Application Module
3. Display Module
#Server Module
The server module is responsible for receiving error messages from the application module and forwarding them to the display module. It listens for incoming UDP messages on a specified port and sends the received messages to another specified port for the display module.

#Application Module
The application module generates error messages based on specific system control keys (Ctrl+C for INFO, Ctrl+Z for WARNING, Ctrl+\ for CRITICAL). It sends these messages to the server module via UDP.

#Display Module
The display module receives error messages from the server module and displays the timestamp, client ID, process ID, and error type.
