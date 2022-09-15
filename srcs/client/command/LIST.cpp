/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/14 17:45:21 by ebellon          ###   ########lyon.fr   */
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
			std::map<std::string, Channel *>::const_iterator chan = _serv->getChannelMap().begin();
			std::map<std::string, Channel *>::const_iterator end = _serv->getChannelMap().end();
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
				if (_serv->getChannelMap().find(*chan) != _serv->getChannelMap().end())
				{
					clicli << RPL_LISTSTART(clicli.getNickname());
					clicli << RPL_LIST(clicli.getNickname(), *chan, std::to_string(_serv->getChannelMap().find(*chan)->second->getNbClients()), _serv->getChannelMap().find(*chan)->second->getTopic());
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
