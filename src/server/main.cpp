#include <SFML/Network.hpp>
#include <Logger/Logger.hpp>
#include <csignal>
#include "../enums.hpp"
#include "main.hpp"

using namespace std;

Logger          logger = Logger();
sf::TcpListener listener;
bool            running = true;

int main(){

    signal(SIGINT, sigintCatcher);

    if (listener.listen(41900) != sf::Socket::Done){
        logger.log(Logger::LogType::Error, "Could not bind to port 41900");
        return 1;
    }

    logger.log("Music control server running on port 41900.");

    while (running){
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done){
            if (!running) break;
            logger.log(Logger::LogType::Error, "Could not accept new connection.");
        }

        sf::Packet packet;
        client.receive(packet);

        sf::Int8 i;
        packet >> i;

        bool ok = true;
        switch (i){
            case MusicCommand::Play:  logger.log("User says \"Play\"");
                                      break;
            case MusicCommand::Pause: logger.log("User says \"Pause\"");
                                      break;
            case MusicCommand::Next:  logger.log("User says \"Next\"");
                                      break;
            case MusicCommand::Prev:  logger.log("User says \"Prev\"");
                                      break;
            case MusicCommand::Get:   logger.log("User says \"Get\"");
                                      break;
            default: ok = false;
                     logger.log(Logger::LogType::Error, "User sent invalid command.");
                     break;
        }

        packet.clear();
        i = ok ? Response::OK : Response::Error;
        packet << i;

        client.send(packet);

        client.disconnect();

    }

    cout << endl; //Because pressing CTRL-C prints ^C to the screen
    logger.log("Interrupt signal received.");
    logger.log("Shutting down now...");

    listener.close();
}

void sigintCatcher(int sig){
    if (sig != SIGINT){
        logger.log(Logger::LogType::Error, "wut");
        return;
    }
    running = false;
    listener.close();
}
