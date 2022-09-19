/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 17:53:09 by emenella          #+#    #+#             */
/*   Updated: 2022/09/19 17:25:38 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/KICK.hpp"
#include "irc/Server.hpp"

KICK::KICK(Server *serv): AuthenticationCommand(serv)
{

}

KICK::KICK(KICK const& src): AuthenticationCommand(src._serv)
{

}

KICK::~KICK()
{

}

int KICK::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
    {
        
        size_t len = std::distance(begin, end);
        if (len > 3)
        {
            std::string channel = *(begin + 1);
            std::string nick = *(begin + 2);
            std::string message;
            for (args_t::iterator it = begin + 3; it != end; it++)
                message += *it + " ";

            Client *client = _serv->findClient(nick);
            Channel *chan = _serv->findChannel(channel);
            if (!client)
            {
                clicli << ERR_NOSUCHNICK(nick);
                return 0;
            }
            if (!chan)
            {
                clicli << ERR_NOSUCHCHANNEL(channel);
                return 0;
            }
            if (chan->isOp(&clicli))
            {
                chan->removeClient(*client);
                return 1;
            }
            else
                clicli << ERR_CHANOPRIVSNEEDED(clicli.getNickname(), chan->getName());
        }
    }
    return 0;
}
