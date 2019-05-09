/*
#include "json.hpp"
// for convenience
using json = nlohmann::json;
*/

/**
 * Web / Server.cpp
 */

#include <iostream>
#include <boost/format.hpp>
#include <sstream>
#include "server.hpp"

namespace web
{
	Server::Server(
		Socket* socket,
		PSoCHandler* psocHandler
	)
		: mq_(MAX_QUEUE_SIZE),
		psocHandler_(psocHandler),
		ws_(nullptr),
		socket_(socket)
	{
		std::cout << "Web::Server:\t Initializing..." << std::endl;

		int port = 9000;

		// On user connection
		socket_->hub_.onConnection([this](
			uWS::WebSocket<uWS::SERVER> *ws,
			uWS::HttpRequest req
		));

		// On user disconnection
		socket_->hub_.onDisconnection([](
			uWS::WebSocket<uWS::SERVER> *ws,
			int code,
			std::string *message,
			size_t length
		));

		// On message received
		socket_->hub_.onMessage([this](
			uWS::WebSocket<uWS::SERVER> *ws,
			std::string* message,
			size_t length,
			uWS::OpCode opCode
		) {
			std::string data = std::string(message, length);

			std::cout << "web::Server:\t Data received: " << data << std::endl;

			/**
             * handle message
             */

				psocHandler_->getMsgQueue()->send(PSoCHandler::ID_SET_TEMP,ID_SET_HUMI, reqMsg);
			});

        
		if (!socket_->hub_.listen(port)) {
			std::cout << "web::Server:\t Error starting on port '" << port << "'." << std::endl;
			socket_->hubStatus_ = socket_->UWS_STATUS_ERROR;
		}

		std::cout << "web::Server:\t Connection is running on port '" << port << "'" << std::endl;
		std::cout << "web::Server:\t External ip '" << "https://10.9.8.2/" << "'" << std::endl;
	}

	void Server::handleMsg(unsigned long id, osapi::Message* msg)
	{
		switch (id) {
            // preset plant type cases
            case ID_PLANT_1:
			{
				

				break;
			}
			case ID_PLANT_2:
			{
            

				break;
			}
			case ID_PLANT_3:
			{
            

				break;
			}
			// Set temp
			case ID_SET_TEMP:
			{
				
				break;
			}
			// set Humi
			case ID_SET_HUMI:
			{
            

				break;
			}
			// update current temp
			case ID_CURRENT_TEMP:
			{
				

                break;
            }
            // update current humi
			case ID_CURRENT_HUMI:
			{
				
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
/*		// Create initial request struct for passing to callbakc
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
		}*/

		// Set req struct parameters
		req.url = url;
		req.pattern = pattern;

		// Call callback with req info as parameter
		callback(req);
	}

}
