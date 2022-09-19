/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:25:38 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/19 20:26:56 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Server.hpp"
#include <cstdlib>
#include <netdb.h>


int main(int ac, char **av)
{
    if (ac < 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return (0);
    }
    try
    {
        char host[256];
        char *IP;
        struct hostent *host_entry;
        gethostname(host, sizeof(host));
        host_entry = gethostbyname(host);
        IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
        std::cout << IP << std::endl;
        bool verbose = false;
        if (ac > 3 && std::strncmp(av[3], "-v", 3) == 0)
            verbose = true;
        int port = std::atoi(av[1]);
        if (port == 0)
        {
            std::cout << "Invalid port : " << av[1] << std::endl;
            return 0;
        }
        Server server(std::atoi(av[1]), av[2], IP, verbose);
        server.start();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}