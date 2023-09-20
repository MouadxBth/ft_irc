/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotManagement.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:58:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 18:40:10 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/time.h>

long	ft_milliseconds(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_msleep(long milliseconds)
{
	long	start;

	start = ft_milliseconds();
	//usleep(milliseconds * 900);
	while (ft_milliseconds() < start + milliseconds)
		usleep(milliseconds);
}

void Bot::listen()
{
    std::cout << "Bot is listening..." << std::endl;
	
    _enabled = true;
	int	 eventsToBeProcessed = 0;

    std::string input;

	while (_enabled)
	{
		eventsToBeProcessed = poll(&_listener, 1, 0);
		
		if (eventsToBeProcessed < 0 && _enabled)
		{
			std::cerr << "Error : An error has occurred during the processing of events of the current connection!\n" 
				<< strerror(errno) 
				<< std::endl;
			continue;
		}

		if (!eventsToBeProcessed)
        {
            ft_msleep(10);
			continue;
        }

		if (_listener.revents & POLLIN && _enabled)
        {
            try {
                input = readInput(_listener);

                if (input.empty())
                {
                    destroyConnection();
                    return ;
                }

                prepareInput(input);
            }
            catch(std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                _enabled = false;
            }
        }

        _listener.revents = 0;
	}
}