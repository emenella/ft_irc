/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:44:27 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/20 16:56:26 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Server.hpp"

Server::Server(int port, std::string password, bool verbose): SocketServer(port, verbose), _password(password)
{
	
	signal(SIGINT, SocketServer::stop);
	_commandes.insert(std::pair<std::string, ACommand*>("NICK", new NICK(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("PASS", new PASS(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("USER", new USER(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("JOIN", new JOIN(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("PING", new PING(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("PART", new PART(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("PRIVMSG", new PRIVMSG(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("NOTICE", new NOTICE(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("KICK", new KICK(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("NAMES", new NAMES(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("TOPIC", new TOPIC(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("LIST", new LIST(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("QUIT", new QUIT(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("MODE", new MODE(this)));
	_commandes.insert(std::pair<std::string, ACommand*>("INVITE", new INVITE(this)));
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
	for (ChannelMap::iterator it = _channels.begin(); it != _channels.end(); ++it)
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
	Client &client = static_cast<Client&>(connection);
	SocketServer::onDisconnection(connection);
	std::cout << "Disconnection IRC of " << client << std::endl;
	fdConnectionMap.erase(connection.getSock());
	delete &client;
}
void Server::onMessage(Connection& connection, std::string const& message)
{
	SocketServer::onMessage(connection, message);
	Client &client = static_cast<Client&>(connection);
	std::cout << "Message from " << client << ": " << message << std::endl;
	parseCommand(message, client);
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

Server::ChannelMap	const & Server::getChannelMap() const
{
	return _channels;
}

int Server::joinChannel(std::string const &name, Client& client)
{
	if (_channels.find(name) != _channels.end())
	{
		if (!_channels.at(name)->getMods().empty() && !_channels.at(name)->isInvit(&client))
		{
			client << ERR_INVITEONLYCHAN(client.getNickname(), name);
			return 0;
		}
		_channels.at(name)->addClient(client);
		if (_channels.at(name)->isInvit(&client))
			_channels.at(name)->removeInvit(client);
		return 1;
	}
	else
	{
		if (name[0] != '#' || name.find("{") != std::string::npos || name.find("[") != std::string::npos || name.find("|") != std::string::npos ||
			name.find("}") != std::string::npos || name.find("]") != std::string::npos || name.find("/") != std::string::npos)
		{
			client << ERR_BADCARCHAN(name);
			return 0;
		}
		_channels.insert(std::pair<std::string, Channel *>(name, new Channel(name, client)));
		return 1;
	}
}

void Server::eraseEmptyChan()
{
	std::map<std::string, Channel*>::const_iterator chan = this->_channels.begin();
	std::vector<std::string> empty_chan;
	while (chan != this->_channels.end())
	{
		if (chan->second->isEmpty())
			empty_chan.push_back(chan->second->getName());
		chan++;
	}
	std::vector<std::string>::const_iterator it = empty_chan.begin();
	while (it != empty_chan.end())
	{
		delete this->_channels.find(*it)->second;
		this->_channels.erase(*it);
		it++;
	}
}

void Server::partChannel(std::string chan, Client& client)
{
	if (_channels.find(chan) != _channels.end())
	{
		_channels.at(chan)->removeClient(client);
		_channels.at(chan)->removeOp(client);
	}
	else
		client << ERR_NOSUCHCHANNEL(client.getNickname(), chan);
	eraseEmptyChan();
}

Channel* Server::findChannel(std::string name)
{
	ChannelMap::iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

Client* Server::findClient(std::string name)
{
	ConnectionMap::iterator end = fdConnectionMap.end();
	for (ConnectionMap::iterator it = fdConnectionMap.begin(); it != end; it++)
	{
		Client *client = static_cast<Client*>(it->second);
		if (client->getNickname() == name)
			return client;
	}
	return NULL;
}