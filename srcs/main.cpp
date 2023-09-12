#include <iostream>
#include <cstdlib>
#include "../include/Server.hpp"
#include <csignal>

Server *server;

static bool isNumber(char *str)
{
    int index;

    index = -1;
    while (str[++index])
        if (!(str[index] >= '0' && str[index] <= '9'))
            return (false);
    if (!index)
        return (false);
    return (true);
}

static int validatePort(int port)
{
    // should think about limiting it to 6665 - 6667
	return (port > 1024 && port < 65537);
}

void signalHandler(int signum) {
    if (server)
        server->disable();
    
    std::exit(signum);
}

int main(int argc, char **argv)
{
    int port;

    signal(SIGINT, signalHandler);

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

    Server	instance(static_cast<size_t>(port), argv[2]);

    server = &instance;

    server->configure();
    server->enable();
    //server.startServer();
    return (0);
}
