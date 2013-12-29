#include <SFML/Network.hpp>
#include <Logger/Logger.hpp>
#include "docopt.c"
#include "../enums.hpp"

using namespace std;

Logger logger = Logger();

int main(int argc, char* argv[]){

    DocoptArgs args = docopt(argc, argv, true, NMC_VERSION);
    sf::Int8 i;
    sf::Packet packet;

    if (args.play)
        i = MusicCommand::Play;
    else if (args.pause)
        i = MusicCommand::Pause;
    else if (args.next)
        i = MusicCommand::Next;
    else if (args.prev)
        i = MusicCommand::Prev;
    else if (args.get)
        i = MusicCommand::Get;
    else {
        logger.log(Logger::LogType::Error, "Command must be given to run!");
        return 1;
    }

    packet << i;

    sf::TcpSocket socket;

    if (socket.connect(sf::IpAddress::LocalHost, 41900) != sf::Socket::Done){
        logger.log(Logger::LogType::Error, "Connection to host failed.");
        return 2;
    }

    socket.send(packet);
    packet.clear();
    socket.receive(packet);

    packet >> i;

    switch (i){
        case Response::OK: logger.log("Success!");
                           break;

        case Response::Error: logger.log(Logger::LogType::Error, "There was an error");
                              break;

        default: logger.log(Logger::LogType::Error, "wut");
    }

    socket.disconnect();

}
