/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 18:49:26 by emenella          #+#    #+#             */
/*   Updated: 2022/09/20 18:39:49 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/PING.hpp"

PING::PING(Server *serv): AuthenticationCommand(serv)
{
}

PING::PING(PING const& src): AuthenticationCommand(src)
{
    if (this != &src)
    {
        this->serv = src.serv;
    }
}
PING::~PING()
{
    
}
int PING::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
    {
        std::size_t len = std::distance(begin, end);
        if (len < 1)
        {
            clicli << ":No parameter";
            return 0;
        }
        for (args_t::iterator it = begin; it != end; it++)
        {
            if (clicli.getServerHostname() == *it || clicli.getServerIp() == *it)
                clicli << "PONG " + clicli.getAddr();
        }
        return 0;
    }
    clicli << ERR_NOTREGISTERED;
    return 0;
}