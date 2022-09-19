/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 17:51:08 by emenella          #+#    #+#             */
/*   Updated: 2022/09/14 18:02:46 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client/AuthenticationCommand.hpp"

class KICK: public AuthenticationCommand
{
    public:
        KICK(Server *serv);
        KICK(KICK const& src);
        ~KICK();
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);  
};