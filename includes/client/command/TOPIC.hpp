/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:27:24 by emenella          #+#    #+#             */
/*   Updated: 2022/05/10 17:23:46 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client/AuthenticationCommand.hpp"
#include "irc/Channel.hpp"

class TOPIC : public AuthenticationCommand
{
    typedef std::vector<std::string> args_t;
    public:
        TOPIC(Server *serv);
        TOPIC(TOPIC const& src);
        ~TOPIC();
		std::string	unparseArg(args_t::iterator begin, args_t::iterator end) const;
        int execute(Client &clicli, args_t::iterator begin, args_t::iterator end);
};