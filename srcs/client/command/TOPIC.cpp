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

int TOPIC::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
	{
		// begin++;
		// if (begin == end)
		// {
		// 	clicli << ERR_NEEDMOREPARAMS("TOPIC");
		// 	return 0;
		// }
		// if (begin + 1 == end)
		// {

		// }
		// while (begin != end)
		// 	begin++;
	}
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
