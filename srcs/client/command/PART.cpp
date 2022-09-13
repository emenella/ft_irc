/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/13 16:19:24 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/PART.hpp"
#include "irc/Server.hpp"

PART::PART(Server *serv): AuthenticationCommand(serv)
{

}

PART::PART(PART const& src): AuthenticationCommand(src)
{

}

PART::~PART()
{

}

int PART::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
		while (++begin != end)
			_serv->partChannel(*begin, clicli);
	else
        clicli << ERR_NOTREGISTERED;
    return 0;
}
