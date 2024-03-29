/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/13 11:31:23 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/NAMES.hpp"
#include "irc/Server.hpp"

NAMES::NAMES(Server *serv): AuthenticationCommand(serv)
{

}

NAMES::NAMES(NAMES const& src): AuthenticationCommand(src)
{

}

NAMES::~NAMES()
{

}

int NAMES::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
		while (++begin != end)
		{
			size_t i = 0;
			size_t pos;
			std::vector<std::string> chanVec;

			while (pos = (*begin).find(',', i), pos != std::string::npos)
			{
				chanVec.push_back((*begin).substr(i, pos - i));
				i = pos + 1;
			}
			chanVec.push_back((*begin).substr(i));

			std::vector<std::string>::iterator chan = chanVec.begin();
			
			while (chan != chanVec.end())
			{
				if (serv->getChannelMap().find(*chan) != serv->getChannelMap().end())
				{
					clicli << RPL_NAMREPLY(clicli.getNickname(), *chan, serv->getChannelMap().find(*chan)->second->listClients());
					clicli << RPL_ENDOFNAMES(clicli.getNickname(), *chan);
				}
				else
					clicli << RPL_ENDOFNAMES(clicli.getNickname(), *chan);
				chan++;
				
			}
		}
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
