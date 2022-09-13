/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 11:01:50 by emenella          #+#    #+#             */
/*   Updated: 2022/09/13 14:59:32 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client/AuthenticationCommand.hpp"


class PRIVMSG: public AuthenticationCommand
{
    public:
        PRIVMSG(Server *serv);
        PRIVMSG(PRIVMSG const& src);
        ~PRIVMSG();
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);
};