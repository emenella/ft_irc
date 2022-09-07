/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 18:46:51 by emenella          #+#    #+#             */
/*   Updated: 2022/09/07 19:35:53 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "client/AuthenticationCommand.hpp"

class PING: public AuthenticationCommand
{
    public:
        PING(Server *serv);
        PING(PING const& src);
        ~PING();
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);
};