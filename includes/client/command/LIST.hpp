/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:24 by emenella          #+#    #+#             */
/*   Updated: 2022/09/13 13:49:00 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client/AuthenticationCommand.hpp"
#include "irc/Channel.hpp"

class LIST : public AuthenticationCommand
{
    typedef std::vector<std::string> args_t;
    public:
        LIST(Server *serv);
        LIST(LIST const& src);
        ~LIST();
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);
};