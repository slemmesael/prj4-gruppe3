
#include "json.hpp"
// for convenience
using json = nlohmann::json;


/**
 * Web / Server.cpp
 */

#include <iostream>
#include <boost/format.hpp>
#include <sstream>

#include "server.hpp"

//Planter
nlohmann::json plante1 = false;
nlohmann::json plante2 = false;
nlohmann::json plante3 = false;
//PlantList
auto PlantList = nlohmann::json::parse"{
  \"Plants1\": "Gulerod",
  \"Plants2\": "Løg",
  \"Plants3\": "Chili",
  \"Temp1\": 14,
  \"Temp2\": 15,
  \"Temp3\": 16,
  \"Fugt1\": 60,
  \"Fugt2\": 65,
  \"Fugt3\": 70
}"
//PlantTypeRec
auto PlantTypeRec = nlohmann::json::parse"{
  \"Temp\": \"14 grader\",
  \"Fugt\": 65
}"
//PlantSetting
auto PlantSettings = nlohmann::json::parse"{
  \"TempS\": t,
  \"FugtS\": f
}"
//ReqData - bare lige en test inden psocHandler_ virker
auto ReqData = nlohmann::json::parse"{
  \"Temp\": \"14 grader\",
  \"Fugt\": 65
}"


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
    socket_->hub_.onConnection([this](
			uWS::WebSocket<uWS::SERVER> *ws,
			uWS::HttpRequest req
		) {
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
				   std::string message,
				   size_t length,
				   uWS::OpCode opCode
				   ) {
			      std::string data = std::string(message,length);

			      std::cout << "web::Server:\t Data received: " << data << std::endl;
/* match path funktioner her */

			    });


    if (!socket_->hub_.listen(port)) {
      std::cout << "web::Server:\t Error starting on port '" << port << "'." << std::endl;
      socket_->hubStatus_ = socket_->UWS_STATUS_ERROR;
    }

    std::cout << "web::Server:\t Connection is running on port '" << port << "'" << std::endl;
    std::cout << "web::Server:\t External ip '" << "https://10.9.8.2/" << "'" << std::endl;
  }

  void Server::handleMsg(uWS::SERVER *ws, osapi::Message *msg)
  {
    auto msg = nlohmann::json::parse(msg);
    std::cout << msg << std::endl;

    switch (msg.at("command")) {
      // preset plant type cases
    case BrugerIndtastTF:
      {
        std::cout << "Brugeren har indtastet temp på:" << msg.at("brugerT") << "grader" << std::endl;
				std::cout << "Brugeren har indtastet fugt på:" << msg.at("brugerF") << "%" << std::endl;
//        psocHandler_->setTemp(stringMatcher.at(msg.at("brugerT"))); noget i den her stil
//        psocHandler_->setFugt(stringMatcher.at(msg.at("brugerF"))); noget i den her stil
	break;
}

    case PlantTypeRec:
      {
        std::cout << "Modtager default plante information" << std::endl;
	      break;
      }

    case PlantList:
      {
        std::cout << "Overfører liste med planter" << std::endl;
        socket_->send(PlantList);
        break;
      }
      // Set temp
    case PlantSetting:
      {
				std::cout << "Modtager planteindstillinger" << std::endl;
	break;
      }
      // set Humi
    case ReqData:
      {
        std::cout << "spørger efter data fra psocHandler_" << std::endl;
        socket_->send(ReqData);

	break;
      }
      // update current temp
    case SetDefPlants1:
      {
				plante1 = true;
        std::cout << "Modtager default planteindstilling 1:" << plante1 << std::endl;
        std::cout << "Temp:" << msg.at("plantT1") << "grader" << std::endl;
        std::cout << "Temp:" << msg.at("plantF1") << "%" << std::endl;
	break;
      }
      // update current humi
    case SetDefPlants2:
      {
        plante2 = true;
        std::cout << "Modtager default planteindstilling 2:" << plante2 << std::endl;
        std::cout << "Temp:" << msg.at("plantT2") << "grader" << std::endl;
        std::cout << "Temp:" << msg.at("plantF2") << "%" << std::endl;
	break;
      }

      case SetDefPlants3:
        {
          plante3 = true;
          std::cout << "Modtager default planteindstilling 3:" << plante3 << std::endl;
          std::cout << "Temp:" << msg.at("plantT3") << "grader" << std::endl;
          std::cout << "Temp:" << msg.at("plantF3") << "%" << std::endl;
      break;
        }

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


void Server::matchPath(std::string url,
		std::string pattern,
		std::function<void(Request req)> callback)
	{
		// Create initial request struct for passing to callbakc
		Request req;

		// Handle pattern param
		std::vector<std::string> tokens;

		std::istringstream f_pattern(pattern.c_str());
		std::string s;

		// Create match tokens from pattern
		while (getline(f_pattern, s, '/')) {
			if (s.length() == 0) continue;
			tokens.push_back(s);
		}

		// Handle url based on the pattern and tokens
		std::vector<std::string> keys;

		std::istringstream f_url(url.c_str());

		// Create keys from url
		while (getline(f_url, s, '/')) {
			if (s.length() == 0) continue;
			keys.push_back(s);
		}

		// Extract params from url based on tokens
		for (size_t i = 0; i < tokens.size(); i++) {
			s = tokens[i];

			// Check if tokens is param
			if (s.at(0) == ':') {
				s.erase(0, 1);
				req.params[s] = keys[i];
			} else if (s != keys[i]) {
				return;
			}
		}

		// Set req struct parameters
		req.url = url;
		req.pattern = pattern;

		// Call callback with req info as parameter
		callback(req);
	}


}
