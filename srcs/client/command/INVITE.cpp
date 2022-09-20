/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/15 17:49:07 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/INVITE.hpp"
#include "irc/Server.hpp"

INVITE::INVITE(Server *serv): AuthenticationCommand(serv)
{

}

INVITE::INVITE(INVITE const& src): AuthenticationCommand(src)
{

}

INVITE::~INVITE()
{

}

int INVITE::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
	{
		begin++;
		if (std::distance(begin, end) < 2)
		{
			clicli << ERR_NEEDMOREPARAMS("INVITE");
			return 0;
		}
		if (serv->getChannelMap().find(*(begin + 1)) == serv->getChannelMap().end())
		{
			clicli << ERR_NOSUCHCHANNEL(clicli.getNickname(), *(begin + 1));
			return 0;
		}
		if (!serv->getChannelMap().find(*(begin + 1))->second->isClient(&clicli))
		{
			clicli << ERR_NOTONCHANNEL(*(begin + 1));
			return 0;
		}
		Client * client = serv->findClient(*begin);
		if (client == NULL)
		{
			clicli << ERR_NOSUCHNICK(*begin);
			return 0;
		}
		if (serv->getChannelMap().find(*(begin + 1))->second->isClient(client))
		{
			clicli << ERR_USERONCHANNEL(clicli.getNickname(), client->getNickname(), *(begin + 1));
			return 0;
		}
		if (!serv->getChannelMap().find(*(begin + 1))->second->getMods().empty() && !serv->getChannelMap().find(*(begin + 1))->second->isOp(&clicli))
		{
			clicli << ERR_CHANOPRIVSNEEDED(clicli.getNickname(), *(begin + 1));
			return 0;
		}
		serv->getChannelMap().find(*(begin + 1))->second->addInvit(*client);
		clicli << RPL_INVITING(clicli.getNickname(), client->getNickname(), *(begin + 1));
		*client << INVITE_MESSAGE(clicli.getNickname(), clicli.getUsername(), clicli.getAddr(), client->getNickname(), *(begin + 1));
		return 1;
	}
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
