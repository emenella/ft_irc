/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:25:38 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/20 16:54:16 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Server.hpp"
#include <cstdlib>


int main(int ac, char **av)
{
    if (ac < 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return (0);
    }
    try
    {
        bool verbose = false;
        if (ac > 3 && std::strncmp(av[3], "-v", 3) == 0)
            verbose = true;
        int port = std::atoi(av[1]);
        if (port == 0)
        {
            std::cout << "Invalid port : " << av[1] << std::endl;
            return 0;
        }
        Server server(std::atoi(av[1]), av[2], verbose);
        server.start();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}