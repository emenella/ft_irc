/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:53:39 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/20 20:26:20 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Channel.hpp"


Channel::Channel(std::string name, Client & clicli) : name(name)
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
		name = rhs.name;
		client = rhs.client;
	}
	return *this;
}
Channel::~Channel()
{
}

std::string	const				Channel::getName() const
{
	return name;
}

std::string	const				Channel::getMods() const
{
	return this->mods;
}

std::string	const				Channel::getTopic() const
{
	return this->topic;
}

size_t							Channel::getNbClients() const
{
	return std::distance(this->clientListBegin(), this->clientListEnd());
}

void							Channel::setTopic(std::string topic)
{
	this->topic = topic;
	std::vector<Client *>::const_iterator it = this->clientListBegin();
	while (it != this->clientListEnd())
	{
		if (!topic.empty())
			*(*it) << RPL_TOPIC((*it)->getNickname(), this->name, this->topic);
		else
			*(*it) << RPL_NOTOPIC((*it)->getNickname(), this->name);
		it++;
	}
}

void							Channel::setMods(std::string mods, Client & clicli)
{
	if (!isOp(&clicli))
	{
		clicli << ERR_CHANOPRIVSNEEDED(clicli.getNickname(), this->name);
		return ;
	}
	if (!(mods[0] == '+' || mods[0] == '-') || mods[1] != 'i')
		return ;
	switch (mods[0])
	{
		case '+':
			if (!mods.empty())
			{
				this->mods = mods;
				std::vector<Client *>::const_iterator it = this->clientListBegin();
				while (it != this->clientListEnd())
				{
					*(*it) << MODE_MESSAGE(clicli.getNickname(), clicli.getUsername(), clicli.getAddr(), this->getName(), mods);
					it++;
				}
				return ;
			}
		case '-':
			if (!mods.empty())
			{
				this->mods.clear();
				std::vector<Client *>::const_iterator it = this->clientListBegin();
				while (it != this->clientListEnd())
				{
					*(*it) << MODE_MESSAGE(clicli.getNickname(), clicli.getUsername(), clicli.getAddr(), this->getName(), mods);
					it++;
				}
				return ;
			}
	}
}

std::vector<Client *>::const_iterator Channel::clientListBegin() const
{
	return client.begin();
}

std::vector<Client *>::const_iterator Channel::clientListEnd() const
{
	return client.end();
}

std::vector<Client *>::const_iterator Channel::opListBegin() const
{
	return opList.begin();
}

std::vector<Client *>::const_iterator Channel::opListEnd() const
{
	return opList.end();
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

bool							Channel::isClient(Client * clicli) const
{
	std::vector<Client *>::const_iterator client = this->clientListBegin();
	while (client != this->clientListEnd())
	{
		if ((*client) == clicli)
			return true;
		client++;
	}
	return false;
}

bool							Channel::isInvit(Client * clicli) const
{
	std::vector<Client *>::const_iterator client = this->invit.begin();
	while (client != this->invit.end())
	{
		if ((*client) == clicli)
			return true;
		client++;
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
	this->client.push_back(&client);
	it = this->clientListBegin();
	while (it != this->clientListEnd())
	{
		*(*it) << JOIN_MESSAGE(client.getNickname(), client.getUsername(), client.getAddr(), this->getName());
		it++;
	}
	if (!this->topic.empty())
		client << RPL_TOPIC(client.getNickname(), this->name, this->topic);
	client << RPL_NAMREPLY(client.getNickname(), this->name, this->listClients());
	client << RPL_ENDOFNAMES(client.getNickname(), this->name);
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
			this->client.erase(it);
			return ;
		}
		it++;
	}
	client << ERR_NOTONCHANNEL(this->getName());
}

void							Channel::addInvit(Client & invit)
{
	std::vector<Client *>::const_iterator it;
	it = this->invit.begin();
	while (it != this->invit.end())
	{
		if ((*it)->getSock() == invit.getSock())
			return ;
		it++;
	}
	this->invit.push_back(&invit);
}

void							Channel::removeInvit(Client & invit)
{
	std::vector<Client *>::const_iterator it;
	it = this->invit.begin();
	while (it != this->invit.end())
	{
		if ((*it)->getSock() == invit.getSock())
		{
			this->invit.erase(it);
			break ;
		}
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
	this->opList.push_back(&op);
}

void							Channel::removeOp(Client& op)
{
	std::vector<Client *>::const_iterator it;
	it = this->opListBegin();
	while (it != this->opListEnd())
	{
		if ((*it)->getSock() == op.getSock())
		{
			this->opList.erase(it);
			break ;
		}
		it++;
	}
	if (this->opList.empty() && !this->client.empty())
		addOp(*this->client[0]);
}

bool							Channel::isEmpty() const
{
	return this->client.empty();
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