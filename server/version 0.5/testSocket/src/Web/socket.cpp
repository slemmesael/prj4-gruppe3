/**
* web / socket.cpp
*/

#include <iostream>
#include "socket.hpp"

namespace Web
{
    Socket::Socket()
        : hubStatus_(UWS_STATUS_VALID)
    {
        // Nothing
    }

    void Socket::run()
    {
        while (1) {
            /** uWebsocket handling */
            // Poll for data on the websocket
            if (hubStatus_ == UWS_STATUS_VALID) {
                hub_.poll();
                // std::cout << "Web::Socket: Polled" << std::endl;
            } else {
                std::cout << "Web::Socket: Can't poll, status invalid" << std::endl;
                break;
            }
        }

        std::cout << "Web::Socket: Thread ended." << std::endl;
    }
}
