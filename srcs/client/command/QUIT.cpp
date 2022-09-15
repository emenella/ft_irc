/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/15 17:03:10 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/QUIT.hpp"
#include "irc/Server.hpp"

QUIT::QUIT(Server *serv): AuthenticationCommand(serv)
{

}

QUIT::QUIT(QUIT const& src): AuthenticationCommand(src)
{

}

QUIT::~QUIT()
{

}

int QUIT::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
	{
		std::map<std::string, Channel *>::const_iterator chan = _serv->getChannelMap().begin();
		std::map<std::string, Channel *>::const_iterator end = _serv->getChannelMap().end();
		while (chan != end)
		{
			if (chan->second->isClient(&clicli))
			{
				chan->second->removeClient(clicli);
				chan->second->removeOp(clicli);
			}
			chan++;
		}
		_serv->eraseEmptyChan();
	}
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
