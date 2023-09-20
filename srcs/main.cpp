#include <iostream>
#include <cstdlib>
#include <csignal>

#include "Server.hpp"
#include "Utilities.hpp"

Server *server;

void signalPipeHandler(int signum)
{
    (void) signum;
}

void signalInterruptHandler(int signum) {
    if (server)
    {
        delete server;
    }
    
    std::exit(signum);
}

int main(int argc, char **argv)
{
    int port;

    signal(SIGINT, signalInterruptHandler);
    signal(SIGPIPE, signalPipeHandler);

    if (argc != 3)
        return (std::cerr << "[Error]: Invalid usage!\n"
            << "Proper usage: (./ircserv <PORT> <PASSWORD>)"
            << std::endl,
            EXIT_FAILURE);
    
    if (!isNumber(argv[1]))
        return (std::cerr << "[Error]: Port must be a number!\n"
                << "Proper usage: (./ircserv <PORT: Positive number> <PASSWORD: String>)"
                << std::endl,
                EXIT_FAILURE);
    
    if ((port = atoi(argv[1])) < 0 || !validatePort(port))
        return (std::cerr << "[Error]: Invalid port!\n"
            << "Proper usage: (./ircserv <PORT: Positive number> <PASSWORD: String>)\n"
            << "=> Port range: [1025-65536]"
            << std::endl,
            EXIT_FAILURE);

    std::string password(argv[2]);

    server = Server::createInstance(static_cast<size_t>(port), password);
    server->enable();
    return (0);
}
