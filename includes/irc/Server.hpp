/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:38:35 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/13 17:34:12 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include "socket/SocketServer.hpp"
#include "irc/Channel.hpp"
#include "client/Client.hpp"
#include "client/command/NICK.hpp"
#include "client/command/PASS.hpp"
#include "client/command/USER.hpp"
#include "client/command/JOIN.hpp"
#include "client/command/PING.hpp"
#include "client/command/PART.hpp"
#include "client/command/NAMES.hpp"
#include "client/command/TOPIC.hpp"

class Server: public SocketServer
{
	typedef std::map<std::string, ACommand*> CommandMap;
	typedef std::map<std::string, Channel*> ChannelMap;
	private:
		std::string		_password;
		CommandMap 		_commandes;
		ChannelMap 		_channels;
		
	public:
		Server(int port, std::string password, std::string hostname, bool verbose = false);
		~Server() throw();

		std::string 	getPassword() const;
		ChannelMap const &	getChannelMap() const;

		void			setPassword(std::string password);

		void			onConnection(int connectionFd, sockaddr_in& address);
		void			onDisconnection(Connection& connection);
		void			onMessage(Connection& connection, std::string const& message);

		void 			parseCommand(std::string const &message, Client& client);
		int 			joinChannel(std::string const &name, Client& client);
		void 			leaveChannel(Client& client);
		void 			partChannel(std::string chan, Client& client);
		void			debugChannel() const;
		ConnectionMap::const_iterator begin() const;
		ConnectionMap::const_iterator end() const;
};