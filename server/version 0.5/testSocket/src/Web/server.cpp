
/**
 * Web / Server.cpp
 */

#include <iostream>
#include <boost/format.hpp>
#include <sstream>
#include <thread>

#include "server.hpp"


#include "json.hpp"
// for convenience
using json = nlohmann::json;

namespace Web
{
  Server::Server(
		 Socket* socket
         )
    : mq_(MAX_QUEUE_SIZE),
      ws_(nullptr),
      socket_(socket)
  {
     std::cout << "Web::Server:\t Initializing..." << std::endl;

    static int totalUsers = 0;
    int port = 9100;


    // On user connection
    socket_->hub_.onConnection([this]
        (
			uWS::WebSocket<uWS::SERVER> *ws,
			uWS::HttpRequest req
		) 
        {
			ws_ = ws;
			totalUsers++;
			std::cout << "Web::Server:\t User connected! Total: " << totalUsers << std::endl;
		});

    // On user disconnection
    socket_->hub_.onDisconnection([](
			uWS::WebSocket<uWS::SERVER> *ws,
			int code,
			char *message,
			size_t length
		) {
			totalUsers--;
			std::cout << "Web::Server:\t User disconnected! Total: " << totalUsers << std::endl;
		});

    // On message received
    socket_->hub_.onMessage([this](
				   uWS::WebSocket<uWS::SERVER> *ws,
				   char* message,
				   size_t length,
				   uWS::OpCode opCode
				   )
                    {
                                          
			     std::string data = std::string(message,length);			      
                std::cout << "web::Server:\t Data received: " << data << std::endl; 
                
                // handle manual settings
                if(length >= 50)
                {
                std::cout << "Web::Server:\t Received request: manual. Redirecting message." << std::endl;
				manuelSettings* reqMsg = new manuelSettings;
				reqMsg->mq = &mq_;
				//reqMsg->returnId = ID_ManualSettings;
                };
            
			    });
        
    if (!socket_->hub_.listen(port)) 
    {
        std::cout << "web::Server:\t Error starting on port '" << port << "'." << std::endl;
        socket_->hubStatus_ = socket_->UWS_STATUS_ERROR;
    }
    std::cout << "web::Server:\t Connection is running on port '" << port << "'" << std::endl;
    std::cout << "web::Server:\t External ip '" << "https://10.9.8.2/" << "'" << std::endl;
  }


  void Server::handleMsg(unsigned long id, osapi::Message* msg)
  {
    switch (id) 
    {
        case 1:
            std::cout<<"manuel settings"<< std::endl;
        break;
        
      // preset plant type cases
    default:
      break;
    }
  }

  void Server::run()
  {
    while (1) {
      // Message ID
      unsigned long id;

      // Receive the message from the queue and handle it
      osapi::Message* msg = mq_.receive(id);
      handleMsg(id, msg);

      // Remove message pointer to prevent memory loss
      delete msg;
    }
  }
  
}
