/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:53:39 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/07 18:32:55 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Channel.hpp"


Channel::Channel(std::string name, Client & clicli) : _name(name)
{
	this->addClient(clicli);
	this->addOp(clicli);
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
			this->_clientList.erase(it);
		it++;
	}
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
			this->_opList.erase(it);
		it++;
	}
}

std::string						Channel::listClients() const
{
	std::string result;
	
	std::vector<Client *>::const_iterator user = this->clientListBegin();
	while (user != this->clientListEnd())
	{
		std::vector<Client *>::const_iterator op = this->opListBegin();
		while (op != this->opListEnd())
		{
			if ((*op) == (*user))
			{
				result += "@";
				break ;
			}
			op++;
		}
		result += (*user)->getNickname() + " ";
		user++;
	}
	result.pop_back();
	return result;
}

std::ostream&                       operator<<(std::ostream& o, Channel const& rhs)
{
	o << "Channel [" << rhs.getName() << "]:" << std::endl;
	int	i = 0;
	for (std::vector<Client *>::const_iterator it = rhs.clientListBegin(); it != rhs.clientListEnd(); ++it)
	{
		o << "\t" << (*it)->getNickname() << "(" << i << ")"<< std::endl;
		i++;
	}
	return o;
}