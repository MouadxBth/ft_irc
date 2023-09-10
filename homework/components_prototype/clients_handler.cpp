#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <vector>
#include <poll.h>
#include <iostream>


int init_listener(void)
{
	sockaddr_in	address;
	int		listener;
	int		opt = 1;

	listener = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(listener, F_SETFL, O_NONBLOCK);
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_port = htons(6667);
	address.sin_addr.s_addr = INADDR_ANY;

	bind(listener, (sockaddr *)&address, sizeof(address));

	listen(listener, SOMAXCONN);

	return(listener);
}


int	handle_client(int	client_sock)
{
	char	buff[512];
	memset(buff, 0, sizeof(buff));
	if (recv(client_sock, buff, sizeof(buff), 0) <= 0)
		return (0);
	else
		std::cout << "client " << client_sock << " sent: " << buff << std::endl;
	return 1;
}



void	establish_connection(void)
{
	std::vector<struct pollfd>	pfds;
	struct pollfd			pfd;
	long	long	fds_count;

	pfd.fd = init_listener();
	pfd.events = POLLIN;
	pfds.push_back(pfd);

	while (1)
	{
		long	long	inc_fds = 0;
		fds_count = poll(&pfds[0], pfds.size(), -1);
		for (long long inx = 0; inx < pfds.size(); inx++)
		{
			if (fds_count == inc_fds)
				break;
			if (pfds[inx].revents & (POLLIN | POLLHUP))
			{
				if (pfds[inx].revents & POLLHUP)
				{
					std::cout << "client " << pfds[inx].fd << " dropped the connection" << std::endl;
					close(pfds[inx].fd);
					pfds.erase(pfds.begin() + inx);
				}
				if (pfds[inx].fd == pfds[0].fd)
				{
					pfd.fd = accept(pfds[0].fd, NULL, NULL);
					pfds.push_back(pfd);
				}
				else
				{
					handle_client(pfds[inx].fd);
					/* if (!handle_client(pfds[inx].fd)) */
					/* { */
					/* 	std::cout << "client " << pfds[inx].fd << " dropped the connection" << std::endl; */
					/* 	close(pfds[inx].fd); */
					/* 	pfds.erase(pfds.begin() + inx); */
					/* } */
				}
				inc_fds++;
			}
		}
	}
}

int main(void)
{
	establish_connection();
	return (0);
}
