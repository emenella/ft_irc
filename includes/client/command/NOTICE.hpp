/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 17:22:36 by emenella          #+#    #+#             */
/*   Updated: 2022/09/14 17:27:02 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client/command/PRIVMSG.hpp"


class NOTICE: public PRIVMSG
{
    public:
        NOTICE(Server *serv);
        NOTICE(NOTICE const& src);
        ~NOTICE();
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);
};