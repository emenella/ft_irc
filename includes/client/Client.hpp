/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 19:30:04 by emenella          #+#    #+#             */
/*   Updated: 2022/09/07 18:35:53 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "irc/Channel.hpp"
#include "socket/SocketConnection.hpp"
#include "client/REPLY.hpp"
#include "socket/SocketServer.hpp"

class Channel;

class Client: public SocketConnection
{
    private:
        std::string                         _nickname;
        std::string                         _username;
        std::string                         _hostname;
        std::string                         _servername;
        std::string                         _serverHostname;
        std::string                         _realname;
        std::string                         _password;
        std::string                         _mode;
        std::map<std::string ,Channel*>     _channels;
        bool                                _register;
        bool                                _op;
        
    public:
        Client(int sock, sockaddr_in &addr, SocketServer &srv);
        Client(Client const &src);
        Client  &operator=(Client const &rhs);
        ~Client() throw();

        void                                setNickname(std::string nickname);
        void                                setUsername(std::string username);
        void                                setHostname(std::string hostname);
        void                                setServerName(std::string nameserver);
        void                                setRealName(std::string realname);
        void                                setPassword(std::string password);
        void                                setRegister(bool bool_);
        
        
        std::string                         getUsername() const;
        std::string                         getNickname() const;
        std::string                         getHostname() const;
        std::string                         getServerName() const;
        std::string                         getServerHostname() const;
        std::string                         getRealName() const;
        std::string                         getPassword() const;
        std::string                         getMode() const;
        std::map<std::string ,Channel*>     getChannels() const;
        bool                                getRegister() const;
        bool                                getOp() const;
        
        void                                readChannels() const;
        
        bool                                isInChannel(std::string name) const;
        void                                updateRegister();

        Client &operator<<(std::string const &reply);
};

std::ostream&                       operator<<(std::ostream& o, Client const& rhs);