/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:25:38 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/13 18:36:35 by emenella         ###   ########.fr       */
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
        int i = 0;
        bool verbose = false;
        while (i < ac)
        {
            if (std::strncmp(av[i], "-v", 3) == 0)
                verbose = true;
            i++;
        }
        Server server(std::atoi(av[1]), av[2], "127.0.0.1", verbose);
        server.start();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}