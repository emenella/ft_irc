/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 22:40:56 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/20 18:47:17 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

Client::Client(int sock, sockaddr_in &addr, SocketServer &srv) : SocketConnection(sock, addr),
											nickname(""), username(""),
											hostname(""), servername(""),
											serverHostname(srv.getHostname()), serverIP(srv.getIP()),
											realname(""), password(""),
											isregister(false), isop(false)
{
}

Client::Client(Client const &rhs) : SocketConnection(rhs)
{
	if (this != &rhs)
	{
		nickname = rhs.nickname;
		username = rhs.username;
		hostname = rhs.hostname;
		servername = rhs.servername;
		realname = rhs.realname;
		password = rhs.password;
		isregister = rhs.isregister;
		isop = rhs.isop;
	}
}

Client::~Client() throw()
{
}

Client								&Client::operator=(Client const &rhs)
{
	(void)rhs;
	return (*this);
}

void                                Client::setNickname(std::string nickname)
{
	this->nickname = nickname;
}

void                                Client::setUsername(std::string username)
{
	this->username = username;
}

void                                Client::setHostname(std::string hostname)
{
	this->hostname = hostname;
}

void                                Client::setServerName(std::string nameserver)
{
	servername = nameserver;
}

void                                Client::setRealName(std::string realname)
{
	this->realname = realname;
}

void                                Client::setPassword(std::string password)
{
	this->password = password;
}

bool 							  Client::getRegister() const
{
	return (isregister);
}	

std::string                         Client::getUsername() const
{
	return username;
}

std::string                         Client::getNickname() const
{
	return nickname;
}

std::string                         Client::getHostname() const
{
	return hostname;
}

std::string                         Client::getServerName() const
{
	return servername;
}

std::string                         Client::getServerIp() const
{
	return serverIP;
}

std::string                         Client::getServerHostname() const
{
	return serverHostname;
}

std::string                         Client::getRealName() const
{
	return realname;
}

std::string                         Client::getPassword() const
{
	return password;
}

std::string                         Client::getMode() const
{
	return mode;
}

std::map<std::string ,Channel*>     Client::getChannels() const
{
	return channels;
}

bool                                Client::getOp() const
{
	return isop;
}

void                                Client::readChannels() const
{
	for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::cout << it->first << std::endl;
	}
}
		
bool                                Client::isInChannel(std::string name) const
{
	for (std::map<std::string ,Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
		if (it->first == name)
			return true;
	return false;
}

std::ostream &						operator<<(std::ostream & o, Client const &rhs)
{
	o << "Client(" << rhs.getNickname() << "): User=" << rhs.getUsername();
	if (rhs.getRegister())
		o << " \033[32m[\033[0mConnected\033[32m]\033[0m ";
	else
		o << " \033[31m/!\\\033[0m ";
	return o;
}

void							Client::setRegister(bool bool_)
{
	isregister = bool_;
}

void Client::updateRegister()
{
	if (isregister)
		return ;
    if (this->getNickname().empty() || this->getPassword().empty() || this->getUsername().empty() || this->getRealName().empty() || this->getHostname().empty())
    {
		return ;
    }
    this->setRegister(true);
	*(this) << RPL_WELCOME(this->getNickname(), this->getUsername(), this->getHostname());
}

Client &Client::operator<<(std::string const &reply)
{
	std::string msg = reply + "\n";
	std::cout << "Message to " << *(this) << ": " << msg;
	SocketConnection::operator<<(msg);
	flush();
	return *this;
}