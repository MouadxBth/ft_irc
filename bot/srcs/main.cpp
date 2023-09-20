/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:35:40 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 18:32:05 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>

# include "Bot.hpp"

bool isNumber(const char *str)
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

int validatePort(int port)
{
    // should think about limiting it to 6665 - 6667
	return (port > 1024 && port < 65537);
}

int main(int argc, char **argv)
{
    int port;
    
    if (argc < 4)
        return (std::cerr << "[Error]: Invalid usage!\n"
            << "Proper usage: (./bot <PORT> <HOST> <PASSWORD>)"
            << std::endl,
            EXIT_FAILURE);
    
    if (!isNumber(argv[1]))
        return (std::cerr << "[Error]: Port must be a number!\n"
                << "Proper usage: (./bot <PORT: Positive number> <HOST: String> <PASSWORD: String>)"
                << std::endl,
                EXIT_FAILURE);
    
    if ((port = atoi(argv[1])) < 0 || !validatePort(port))
        return (std::cerr << "[Error]: Invalid port!\n"
            << "Proper usage: (./bot <PORT: Positive number> <HOST: String> <PASSWORD: String>)\n"
            << "=> Port range: [1025-65536]"
            << std::endl,
            EXIT_FAILURE);

    std::string host(argv[2]);
    std::string password(argv[3]);

    Bot bot(password);

    bot.configure(port, host);
    bot.establishConnection();
    bot.listen();
    return 0;
}
