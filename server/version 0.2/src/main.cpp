/**
 * @file main.cpp
 *
 * @brief Default main file for the project
 *
 * @date 2010.05.09
 * @version 0.1
 */

#include <iostream>


#include "web/server.hpp"
#include "web/socket.hpp"
#include "PSoC_handler.hpp"


int main(int argc, char* argv[]) {

    /** I2C File Initialization */
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    int file;

    const char *filename = "/dev/i2c-1";

    if ((file = open(filename, filePerms)) < 0) {
        printf("Failed to open file '%s'\n", filename);
    }

    PSoCHandler psoc(&file);
    Web::Socket socket;
    Web::Server server(&socket, &lh, &psoc);

/*  osapi::Thread serverThread(&server);
    osapi::Thread socketThread(&socket);
    osapi::Thread psocThread(&psoc);

    serverThread.start();
    socketThread.start();
    psocThread.start();

    serverThread.join();
    socketThread.join();
    psocThread.join();
*/

    close(file);

    return 0;
}
