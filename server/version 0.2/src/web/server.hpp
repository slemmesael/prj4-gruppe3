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

/*#include <osapi/Message.hpp>
#include <osapi/MsgQueue.hpp>
#include <osapi/ThreadFunctor.hpp>
*/
#include <string>
#include <vector>

#include "web/socket.hpp"
#include "PSoC_handler.hpp"

namespace web
{
    /**
     * Request struct for returning the valid information on a callback
     * validation
     */
    struct Request
    {
        std::string url;
        std::string pattern;
    };

    class Server : public osapi::ThreadFunctor
    {
    public:
        /**
         * Annonymous enumerate that holds the class message ids
         */
        enum {
            /** Plant types */
            ID_PLANT_1, 
            ID_PLANT_2, 
            ID_PLANT_3, 

            /** temperature */
            ID_SET_TEMP,
            ID_CURRENT_TEMP,

            /** humidity */
            ID_SET_HUMI,
            ID_CURRENT_HUMI,
            
            

        };

        /**
         * Class constructor that initializes all the local reference to other
         * object instances.
         * @param socket       Socket reference
         * @param psocHandler  PSoCHandler reference
         */
        Server(Socket* socket,
            PSoCHandler* psocHandler);

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
         * The matching function. Used just like a regular routing function in
         * any other language (mainly js). Pass the current url and then a
         * specific pattern to match. If the url matches the pattern, the
         * callback will be run and parsed with the match information as a
         * request struct.
         * @param url      The current parsed url
         * @param pattern  The pattern to match
         * @param callback The callback function to be called, if the url
         *                 mathces the pattern.
         */
        void matchPath(std::string url,
            std::string pattern,
            std::function<void(Request req)> callback);

        /**
         * Reference to the PSoCHandler instance created by main and parsed by
         * to this object by the constructor.
         */
        PSoCHandler* psocHandler_;

        /**
         * The active µWebsocket connection that is initialized in onConnection
         */
        uWS::WebSocket<uWS::SERVER>* ws_;
        /**
         * A local reference to a Socket instance created by main and parsed by
         * the class constructor.
         */
        Socket* socket_;

        /**
         * Current temperature
         */
        int temp_;

        /**
         * Current humidity
         */
        int humi_;

    };
}


#endif /* end of include guard: WEB_SERVER_HPP */
