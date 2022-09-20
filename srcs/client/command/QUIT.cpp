/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/20 18:39:49 by emenella         ###   ########.fr       */
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
		std::map<std::string, Channel *>::const_iterator chan = serv->getChannelMap().begin();
		std::map<std::string, Channel *>::const_iterator end = serv->getChannelMap().end();
		while (chan != end)
		{
			if (chan->second->isClient(&clicli))
			{
				chan->second->removeClient(clicli);
				chan->second->removeOp(clicli);
			}
			chan++;
		}
		serv->eraseEmptyChan();
	}
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
