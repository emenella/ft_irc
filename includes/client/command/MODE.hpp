/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:24 by emenella          #+#    #+#             */
/*   Updated: 2022/09/15 17:26:57 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client/AuthenticationCommand.hpp"
#include "irc/Channel.hpp"

class MODE : public AuthenticationCommand
{
    typedef std::vector<std::string> args_t;
    public:
        MODE(Server *serv);
        MODE(MODE const& src);
        ~MODE();
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);
};