#include <SFML/Network.hpp>
#include <Logger/Logger.hpp>
#include <csignal>
#include "../enums.hpp"
#include "docopt.c"
#include "main.hpp"

using namespace std;

Logger          logger = Logger();
sf::TcpListener listener;
bool            running = true;

int main(int argc, char* argv[]){

    signal(SIGINT, sigintCatcher);

    DocoptArgs args = docopt(argc, argv, true, NMC_VERSION);

    //Since docopt is sucking at defaults
    if (args.port == NULL)
        args.port = (char *) "41900"; //It's supposed to come out as a string, so might as well set the default as a string.

    unsigned short port;
    if (!(stringstream(args.port) >> port)){
        logger.log(Logger::LogType::Error, "Invalid port number.");
        return 2;
    }

    if (listener.listen(port) != sf::Socket::Done){
        logger.log(Logger::LogType::Error, "Could not bind to port ", port);
        return 1;
    }

    logger.log("Music control server running on port ", port, ".");

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
