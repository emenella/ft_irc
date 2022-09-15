/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:24 by emenella          #+#    #+#             */
/*   Updated: 2022/09/15 15:58:14 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client/AuthenticationCommand.hpp"
#include "irc/Channel.hpp"

class QUIT : public AuthenticationCommand
{
    typedef std::vector<std::string> args_t;
    public:
        QUIT(Server *serv);
        QUIT(QUIT const& src);
        ~QUIT();
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);
};