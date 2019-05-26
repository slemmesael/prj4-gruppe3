
/**
 * Web / Server.cpp
 */

#include <iostream>
#include <boost/format.hpp>
#include <sstream>
#include <thread>
#include <string>

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
				
                /* test socket 
               if (length > 0 )
               {
                                     
                std::string m = "message received"; 
                     
                ws_->send(m.c_str(), m.size(), uWS::OpCode::TEXT);
               }
                */
                
                json dataParse = json::parse(data);
                auto findCommand = dataParse.json::find("command");
                std::cout << "Web::Server:\t parsed data: " << dataParse << std::endl;
                
                std::cout << "Web::Server:\t kommando: " << *findCommand << std::endl;
                // handle manual settings
                if (*findCommand =="BrugerIndtastTF")
                {
                        std::cout<<"Web::Server:\t BrugerIndtastTF modtaget" << std::endl;
                        auto temp= dataParse.json::find("brugerT");
                        auto humi= dataParse.json::find("brugerF");
                        
                        std::string tmp = *temp;
                        tmp.erase(remove(tmp.begin(),tmp.end(),'\"' ), tmp.end());
                        int t = std::stoi(tmp);
                        std::string hum = *humi;
                        hum.erase(remove(hum.begin(),hum.end(),'\"' ), hum.end());
                        int h = std::stoi(hum);
                        
                        std::cout << "Web::Server:\t temp: "<< t << "humi: " << h << std::endl;
                }
                
                // handle presets
                
                if (*findCommand =="PlantList")
                {
                        // send json plant list
                     std::cout << "Web::Server:\t Plantlist request" << std::endl;
                     
                     auto PlantList = json::parse("{\"Plants1\": \"Gulerod\",\"Plants2\": \"Løg\",\"Plants3\": \"Chili\",\"Temp1\": 15, \"Temp2\": 15,\"Temp3\": 25,\"Fugt1\": 45,\"Fugt2\": 50,\"Fugt3\": 55 }");
                                        
                     std::string message = PlantList.dump(); 
                     
                    ws_->send(message.c_str(), message.size(), uWS::OpCode::TEXT);
                }
                else if (*findCommand =="SetDefPlants1")
                {
                        // set data
                    std::cout << "Web::Server:\t plant 1 " << std::endl;
 

                }
                else if (*findCommand =="SetDefPlants2")
                {
                        // set data
                     std::cout << "Web::Server:\t plant 2 " << std::endl;
                }
                else if (*findCommand =="SetDefPlants3")
                {
                         // set data
                     std::cout << "Web::Server:\t plant 3 " << std::endl;
                }
                else if (*findCommand =="ReqData")
                {
                    //readI2C(temp,humi)
                    auto liveData = json::parse("{\"Temp\": 15, \"Fugt\": 60 }");
                    std::cout << "Web::Server:\t live data sent " << std::endl;
                     
                    std::string message = liveData.dump(); 
                     
                    ws_->send(message.c_str(), message.length(), uWS::OpCode::TEXT);
                    
                }
                else
                    std::cout<<"no valid commando is received" << std::endl;
                                
                
            
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
