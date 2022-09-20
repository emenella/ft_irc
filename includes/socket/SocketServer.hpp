/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:31:47 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/20 18:27:36 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/poll.h>
#include <netdb.h>
#include "socket/SocketConnection.hpp"
#include "socket/SocketListener.hpp"

#define TIMEOUT 3*60*1000

class SocketServer: public SocketListener
{
    public:
        typedef SocketConnection                Connection;
        typedef	std::pair<int, Connection*>     ConnectionPair;
	    typedef	std::map<int, Connection*>		ConnectionMap;
        
        SocketServer(std::string const& hostname, int service, bool verbose);
        SocketServer(int service, bool verbose);
        SocketServer(SocketServer const &src);
        SocketServer &operator=(SocketServer const &rhs);
        ~SocketServer();

        virtual void	    onConnection(int connectionFd, sockaddr_in& address);
	    virtual void		onDisconnection(Connection& connection);
	    virtual void		onMessage(Connection& connection, std::string const& message);

        void start();
        static void stop(int code);
        void receiveAndSend(Connection& connection);
        void poll();
        void listen();
        std::string getHostname() const;
        std::string getIP() const;
     private:
        static bool isRunning;
        bool isVerbose;
        void pushFd(int fd, int events);
        void popFd(int fd);
    protected:
        std::string                             hostname;
        std::string                             IP;
        int                                     service;
        
        struct sockaddr_in                      addr;
        ConnectionMap		                    fdConnectionMap;

        std::vector<pollfd>                     pollFds;
        int                                     timeout;



};