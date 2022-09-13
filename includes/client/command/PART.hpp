/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:24 by emenella          #+#    #+#             */
/*   Updated: 2022/09/08 17:57:36 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client/AuthenticationCommand.hpp"
#include "irc/Channel.hpp"

class PART : public AuthenticationCommand
{
    typedef std::vector<std::string> args_t;
    public:
        PART(Server *serv);
        PART(PART const& src);
        ~PART();
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);
};