/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/20 18:39:49 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/MODE.hpp"
#include "irc/Server.hpp"

MODE::MODE(Server *serv): AuthenticationCommand(serv)
{

}

MODE::MODE(MODE const& src): AuthenticationCommand(src)
{

}

MODE::~MODE()
{

}

int MODE::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
	{
		begin++;
		if (begin == end)
		{
			clicli << ERR_NEEDMOREPARAMS("MODE");
			return 0;
		}
		if (serv->getChannelMap().find(*begin) == serv->getChannelMap().end())
		{
			clicli << ERR_NOSUCHCHANNEL(clicli.getNickname(), *begin);
			return 0;
		}
		if (begin + 1 == end)
		{
			clicli << RPL_CHANNELMODEIS(clicli.getNickname(), *begin, serv->getChannelMap().find(*begin)->second->getMods());
			return 1;
		}
		serv->getChannelMap().find(*begin)->second->setMods(*(begin + 1), clicli);
	}
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
