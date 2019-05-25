/**
 * @file main.cpp
 *
 * @brief Default main file for the project
 *
 * @date 2010.05.09
 * @version 0.1
 */
#include <iostream>

#include "Web/socket.hpp"
#include "Web/server.hpp"
#include "Thread.hpp"
#include "ThreadFunctor.hpp"
#include "regulering.hpp"
using namespace std;


int main(int argc, char* argv[]) 
{
 /*   /** I2C File Initialization */
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    int file;

    const char *filename = "/dev/i2c-1";

    if ((file = open(filename, filePerms)) < 0) 
    {
        printf("Failed to open file '%s'\n", filename);
    }
    
    regulering reg(&file);
*/
    Web::Socket socket_;
    Web::Server server_(&socket_);

    osapi::Thread socketThread(&socket_);
    osapi::Thread serverThread(&server_);
  //  osapi::Thread regThread(&reg);

    socketThread.start();
    serverThread.start();
   // regThread.start();

    socketThread.join();
    serverThread.join();
 //   regThread.join();
    
    close(file)


    return 0;
}
