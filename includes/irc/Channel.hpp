/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 21:32:08 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/07 17:58:55 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "client/Client.hpp"
#include <vector>

class Client;

class Channel
{
	typedef std::vector<Client *> clientList;
	private:
		std::string					_name;
		std::string					_topic;
		std::string					_mods;
		clientList					_opList;
		clientList					_clientList;
		
	public:
		Channel(std::string name, Client & clicli);
		Channel(Channel const &src);
		Channel &operator=(Channel const & rhs);
		~Channel();
		
		std::string	const				getName() const;
		std::string const				getTopic() const;
		std::string	const				getMods() const;
		clientList::const_iterator		clientListBegin() const;
		clientList::const_iterator		clientListEnd() const;
		clientList::const_iterator		opListBegin() const;
		clientList::const_iterator		opListEnd() const;

		void							setTopic(std::string topic);
		void							setMods(std::string mods, Client & clicli);
		void							addClient(Client & clicli);
		void							removeClient(Client & clicli);
		void							addOp(Client & op);
		void							removeOp(Client & op);
		std::string						listClients() const;
};

std::ostream&                       operator<<(std::ostream& o, Channel const& rhs);
