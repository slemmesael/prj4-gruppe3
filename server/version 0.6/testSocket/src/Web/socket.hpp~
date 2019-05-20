/**
 * @file Socket.hpp
 * Web / Socket.hpp
 */

#ifndef WEB_SOCKET_HPP
#define WEB_SOCKET_HPP

#include <osapi/ThreadFunctor.hpp>
#include <osapi/Message.hpp>

#include <uWS/uWS.h>

namespace web
{
    /**
     * Forward declaration
     */
    class Server;
    
    /**
     * Handling only the socket connection.
     */
    class Socket : public osapi::ThreadFunctor
    {
    public:
        /**
         * Annonymous enumerate that holds the class message ids
         */
        enum {
            UWS_STATUS_VALID,
            UWS_STATUS_ERROR,
        };
        /**
         * A constant that defines the max amount of messages that can be in the
         * local osapi message queue.
         */
        static const int MAX_QUEUE_SIZE = 10;
        /**
         * Default constructor
         */
        Socket();
    private:
        /**
         * @brief Gives the Server class access to private members
         * @relates Server
         */
        friend class Server;

        /**
         * Reference
         */
        Server* server_;

        /**
         * The ThreadFunctors thread method.
         */
        void run();

        /**
         * µWebSocket hub for creating the connection and setting up event
         * listeners like onConnection and onMessage.
         */
        uWS::Hub hub_;
        /**
         * Error validation variable. Set to other than 1 if error
         */
        unsigned int hubStatus_;
    };
}


#endif /* end of include guard: WEB_SOCKET_HPP */
