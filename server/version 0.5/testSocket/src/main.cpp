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


int main(int argc, char* argv[]) {

    cout << "main entered" << endl;
    
    Web::Socket socket;
    Web::Server server(&socket);

    osapi::Thread socketThread(&socket);
    cout << "thread is created" << endl;
    osapi::Thread serverThread(&server);
    
    socketThread.start();
    cout << "socket is started" <<endl;
    serverThread.start();
    
    socketThread.join();
    cout << "socket is joined" << endl;
    serverThread.join();
    
    
    return 0;
}
