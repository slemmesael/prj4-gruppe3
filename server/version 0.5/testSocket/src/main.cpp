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
using namespace std;


int main(int argc, char* argv[])
{
    Web::Socket socket;
    Web::Server server(&socket);

    osapi::Thread socketThread(&socket);
    osapi::Thread serverThread(&server);

    socketThread.start();
    serverThread.start();
    
    socketThread.join();
    serverThread.join();
        
    
    return 0;
}
