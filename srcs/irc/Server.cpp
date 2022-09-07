/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:44:27 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/07 19:44:05 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Server.hpp"

Server::Server(int port, std::string password, std::string hostname) : SocketServer(hostname, port), _password(password)
{
	_commandes.insert(std::pair<std::string, ACommand*>("NICK", new NICK(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("PASS", new PASS(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("USER", new USER(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("JOIN", new JOIN(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("PING", new PING(this)));
}

Server::~Server() throw()
{
	for (ConnectionMap::iterator it = fdConnectionMap.begin(); it != fdConnectionMap.end(); ++it)
	{
		delete it->second;
	}
	for (CommandMap::iterator it = _commandes.begin(); it != _commandes.end(); ++it)
	{
		delete it->second;
	}
}

std::string 	Server::getPassword() const
{
	return _password;
}

void			Server::setPassword(std::string password)
{
	_password = password;
}

void Server::onConnection(int connectionFd, sockaddr_in& address)
{
	SocketServer::onConnection(connectionFd, address);
    Client *tmp = new Client(connectionFd, address, *this);
	std::cout << "New connection IRC from " << *tmp << std::endl;
    fdConnectionMap.insert(std::pair<int, Client*>(connectionFd, tmp));
}
void Server::onDisconnection(Connection& connection)
{
	SocketServer::onDisconnection(connection);
	Client &client = static_cast<Client&>(connection);
	std::cout << "Disconnection IRC of " << client << std::endl;
	fdConnectionMap.erase(connection.getSock());
	delete &client;
}
void Server::onMessage(Connection& connection, std::string const& message)
{
	SocketServer::onMessage(connection, message);
	if (message == "EXIT")
	{
		stop();
	}
	Client &client = static_cast<Client&>(connection);
	std::cout << "Message from " << client << ": " << message << std::endl;
	parseCommand(message, client);
}

void Server::debugChannel() const
{
	ChannelMap::const_iterator it = this->_channels.begin();
	while (it != this->_channels.end())
	{
		Channel * chan = it->second;
		std::cout << "chan [" << chan->getName() << "]" << std::endl;
		std::cout << "chan mods : " << chan->getMods() << std::endl;
		std::cout << "clients connected :" << std::endl;
		std::vector<Client *>::const_iterator it_chan = chan->clientListBegin();
		while (it_chan != chan->clientListEnd())
		{
			std::cout << "	" <<(*it_chan)->getNickname() << std::endl;
			it_chan++;
		}
		std::cout << "operators :" << std::endl;
		it_chan = chan->opListBegin();
		while (it_chan != chan->opListEnd())
		{
			std::cout << "	" <<(*it_chan)->getNickname() << std::endl;
			it_chan++;
		}
		it++;
	}
}

void Server::parseCommand(std::string const &message, Client& client)
{
	size_t i = 0;
	size_t pos;
	std::vector<std::string> str;

	while (pos = message.find(' ', i), pos != std::string::npos)
	{
		str.push_back(message.substr(i, pos - i));
		i = pos + 1;
	}
	str.push_back(message.substr(i));
	CommandMap::iterator it = _commandes.find(str[0]);
	if (it != _commandes.end())
	{
		ACommand *command = it->second;
		command->execute(client, str.begin(), str.end());
	}
}

int Server::joinChannel(std::string const &name, Client& client)
{
	// TO DO : test mods and acces before adding clicli
	if (_channels.find(name) != _channels.end())
	{
		_channels.at(name)->addClient(client);
		return 1;
	}
	else
	{
		_channels.insert(std::pair<std::string, Channel *>(name, new Channel(name, client)));
		return 1;
	}
}

int Server::leaveChannel(std::string const &name, Client& client)
{
	if (_channels.find(name) != _channels.end())
	{
		_channels.at(name)->removeClient(client);
		return 1;
	}
	return 0;
}

std::map<int, SocketConnection*>::const_iterator Server::begin() const
{
	return fdConnectionMap.begin();
}

std::map<int, SocketConnection*>::const_iterator Server::end() const
{
	return fdConnectionMap.end();
}