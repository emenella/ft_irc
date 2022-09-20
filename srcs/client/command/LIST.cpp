/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/20 18:39:49 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/LIST.hpp"
#include "irc/Server.hpp"

LIST::LIST(Server *serv): AuthenticationCommand(serv)
{

}

LIST::LIST(LIST const& src): AuthenticationCommand(src)
{

}

LIST::~LIST()
{

}

int LIST::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
	{
		if (begin + 1 == end)
		{
			std::map<std::string, Channel *>::const_iterator chan = serv->getChannelMap().begin();
			std::map<std::string, Channel *>::const_iterator end = serv->getChannelMap().end();
			while (chan != end)
			{
				clicli << RPL_LISTSTART(clicli.getNickname());
				clicli << RPL_LIST(clicli.getNickname(), chan->second->getName(), std::to_string(chan->second->getNbClients()), chan->second->getTopic());
				clicli << RPL_LISTEND(clicli.getNickname());
				chan++;
			}
		}
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
					clicli << RPL_LISTSTART(clicli.getNickname());
					clicli << RPL_LIST(clicli.getNickname(), *chan, std::to_string(serv->getChannelMap().find(*chan)->second->getNbClients()), serv->getChannelMap().find(*chan)->second->getTopic());
					clicli << RPL_LISTEND(clicli.getNickname());
				}
				else
					clicli << ERR_NOSUCHCHANNEL(clicli.getNickname(), *chan);
				chan++;
				
			}
		}
	}
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
