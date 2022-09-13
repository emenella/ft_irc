/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:53:39 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/13 17:51:41 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Channel.hpp"


Channel::Channel(std::string name, Client & clicli) : _name(name)
{
	this->addOp(clicli);
	this->addClient(clicli);
}
Channel::Channel(Channel const &src)
{
	*this = src;
}
Channel & Channel::operator=(Channel const &rhs)
{
	if (this != &rhs)
	{
		_name = rhs._name;
		_clientList = rhs._clientList;
	}
	return *this;
}
Channel::~Channel()
{
}

std::string	const				Channel::getName() const
{
	return _name;
}

std::string	const				Channel::getMods() const
{
	return this->_mods;
}

std::string	const				Channel::getTopic() const
{
	return this->_topic;
}

void							Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

std::vector<Client *>::const_iterator Channel::clientListBegin() const
{
	return _clientList.begin();
}

std::vector<Client *>::const_iterator Channel::clientListEnd() const
{
	return _clientList.end();
}

std::vector<Client *>::const_iterator Channel::opListBegin() const
{
	return _opList.begin();
}

std::vector<Client *>::const_iterator Channel::opListEnd() const
{
	return _opList.end();
}

bool							Channel::isOp(Client * clicli) const
{
	std::vector<Client *>::const_iterator op = this->opListBegin();
	while (op != this->opListEnd())
	{
		if ((*op) == clicli)
			return true;
		op++;
	}
	return false;
}

std::string						Channel::listClients() const
{
	std::string result;
	
	std::vector<Client *>::const_iterator user = this->clientListBegin();
	while (user != this->clientListEnd())
	{
		if (isOp(*user))
			result += "@";
		result += (*user)->getNickname() + " ";
		user++;
	}
	result.pop_back();
	return result;
}

void							Channel::addClient(Client& client)
{
	std::vector<Client *>::const_iterator it;
	it = this->clientListBegin();
	while (it != this->clientListEnd())
	{
		if ((*it)->getSock() == client.getSock())
			return ;
		it++;
	}
	this->_clientList.push_back(&client);
	it = this->clientListBegin();
	while (it != this->clientListEnd())
	{
		*(*it) << JOIN_MESSAGE(client.getNickname(), client.getUsername(), client.getAddr(), this->getName());
		it++;
	}
	client << RPL_NAMREPLY(client.getNickname(), this->getName(), this->listClients());
	client << RPL_ENDOFNAMES(client.getNickname(), this->getName());
}

void							Channel::removeClient(Client& client)
{
	std::vector<Client *>::const_iterator it;
	it = this->clientListBegin();
	while (it != this->clientListEnd())
	{
		if ((*it)->getSock() == client.getSock())
		{
			std::vector<Client *>::const_iterator chan_user = this->clientListBegin();
			while (chan_user != this->clientListEnd())
			{
				*(*chan_user) << PART_MESSAGE(client.getNickname(), client.getUsername(), client.getAddr(), this->getName());
				chan_user++;
			}
			this->_clientList.erase(it);
			return ;
		}
		it++;
	}
	client << ERR_NOTONCHANNEL(this->getName());
}

void							Channel::addOp(Client& op)
{
	std::vector<Client *>::const_iterator it;
	it = this->opListBegin();
	while (it != this->opListEnd())
	{
		if ((*it)->getSock() == op.getSock())
			return ;
		it++;
	}
	this->_opList.push_back(&op);
}

void							Channel::removeOp(Client& op)
{
	std::vector<Client *>::const_iterator it;
	it = this->opListBegin();
	while (it != this->opListEnd())
	{
		if ((*it)->getSock() == op.getSock())
		{
			this->_opList.erase(it);
			return ;
		}
		it++;
	}
}

bool							Channel::isEmpty() const
{
	return this->_clientList.empty();
}

void							Channel::message(Client &client, std::string msg)
{
	clientList::const_iterator end = this->clientListEnd();
	for (clientList::const_iterator it = this->clientListBegin(); it != end; it++)
		if (&**it != &client)
			**it << msg;
}

Channel &Channel::operator<<(std::string const &reply)
{
	clientList::const_iterator end = this->clientListEnd();
	for (clientList::const_iterator it = this->clientListBegin(); it != end; it++)
		*(*it) << reply;
	return *this;
}