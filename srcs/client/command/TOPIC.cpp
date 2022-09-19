/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/13 13:56:23 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/TOPIC.hpp"
#include "irc/Server.hpp"

TOPIC::TOPIC(Server *serv): AuthenticationCommand(serv)
{

}

TOPIC::TOPIC(TOPIC const& src): AuthenticationCommand(src)
{

}

TOPIC::~TOPIC()
{

}

std::string TOPIC::unparseArg(args_t::iterator begin, args_t::iterator end) const
{
	std::string topic;

	for (args_t::iterator it = begin; it != end; it++)
        topic += *it + " ";
    topic.pop_back();
	topic.erase(0, 1);
	return topic;
}

int TOPIC::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
	{
		begin++;
		if (begin == end)
		{
			clicli << ERR_NEEDMOREPARAMS("TOPIC");
			return 0;
		}
		if (begin + 1 == end)
		{
			if (_serv->getChannelMap().find(*begin) != _serv->getChannelMap().end())
			{
				if (_serv->getChannelMap().find(*begin)->second->isClient(&clicli))
				{
					if (!_serv->getChannelMap().find(*begin)->second->getTopic().empty())
						clicli << RPL_TOPIC(clicli.getNickname(), *begin, _serv->getChannelMap().find(*begin)->second->getTopic());
					else
						clicli << RPL_NOTOPIC(clicli.getNickname(), *begin);
				}
				else
					clicli << ERR_NOTONCHANNEL(*begin);
			}
			else
				clicli << ERR_NOSUCHCHANNEL(clicli.getNickname(), *begin);
		}
		else
		{
			if (_serv->getChannelMap().find(*begin) != _serv->getChannelMap().end())
			{
				if (_serv->getChannelMap().find(*begin)->second->isClient(&clicli))
				{
					if (_serv->getChannelMap().find(*begin)->second->isOp(&clicli))
						_serv->getChannelMap().find(*begin)->second->setTopic(unparseArg(begin + 1, end));
					else
						clicli << ERR_CHANOPRIVSNEEDED(clicli.getNickname(), *begin);
				}
				else
					clicli << ERR_NOTONCHANNEL(*begin);
			}
			else
				clicli << ERR_NOSUCHCHANNEL(clicli.getNickname(), *begin);
		}
	}
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
