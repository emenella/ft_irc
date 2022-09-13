/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 11:04:28 by emenella          #+#    #+#             */
/*   Updated: 2022/09/13 11:06:14 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/client/command/PRIVMSG.hpp"

PRIVMSG::PRIVMSG(Server *serv): AuthenticationCommand(serv)
{

}

PRIVMSG::PRIVMSG(PRIVMSG const& src)
{
    if (this != &src)
    {
        this->_serv = src._serv;
    }
}

PRIVMSG::~PRIVMSG()
{

}

int PRIVMSG::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
    {
        
    }
}
