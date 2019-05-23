/**
* @file web/server.hpp
*
* @brief The brain of the system. Manages all the actions through-out the
*        system based on the information sent/received from the web-interface
*        via the µWebsocket connection.
*
* @version 0.1
* @date 2019.05.09
*/

#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <osapi/Message.hpp>
#include <osapi/MsgQueue.hpp>
#include <osapi/ThreadFunctor.hpp>

#include <string>
#include <vector>
#include <map>

#include "Web/socket.hpp"

namespace Web
{
    
    struct manuelSettings : public osapi::Message
    {
    osapi::MsgQueue* mq;
    unsigned int returnId =1;
    };

    struct preSettings : public osapi::Message
    {
    osapi::MsgQueue* mq;
    unsigned int returnId;
    };
    
    class Server : public osapi::ThreadFunctor
    {
    public:
        /**
         * Annonymous enumerate that holds the class message ids
         */
        enum {
            /** Controls */
            ID_ManualSettings,
            ID_PreSettings
        };
        

        /*
         * Class constructor that initializes all the local reference to other
         * object instances.
         * @param socket       Socket reference
         */
        Server(Socket* socket);

        /**
         * A constant that defines the max amount of messages that can be in
         * the local osapi message queue.
         */
        static const int MAX_QUEUE_SIZE = 10;

        /**
         * A simple getter for the osapi message queue
         * @return A reference to the local osapi message queue
         */
        osapi::MsgQueue* getMsgQueue()
        {
            return &mq_;
        }
                

    private:
        /**
         * Handles all the message queue calls. Mostly consists of responses
         * to initial requests that this class has made.
         * @param id  Message id (local)
         * @param msg Message reference
         */
        void handleMsg(unsigned long id, osapi::Message* msg);
        /**
         * The ThreadFunctors thread method.
         */
        void run();

        /**
         * The osapi Message queue
         */
        osapi::MsgQueue mq_;
        
        

       /**
         * The active µWebsocket connection that is initialized in onConnection
         */
        uWS::WebSocket<uWS::SERVER>* ws_;
        /**
         * A local reference to a Socket instance created by main and parsed by
         * the class constructor.
         */
        Socket* socket_;

    };
}


#endif /* end of include guard: WEB_SERVER_HPP */
