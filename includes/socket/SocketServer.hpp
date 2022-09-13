/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:31:47 by bmangin           #+#    #+#             */
/*   Updated: 2022/09/13 18:57:34 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/poll.h>
#include "socket/SocketConnection.hpp"
#include "socket/SocketListener.hpp"

#define TIMEOUT 3*60*1000

class SocketServer: public SocketListener
{
    public:
        typedef SocketConnection                Connection;
        typedef	std::pair<int, Connection*>     ConnectionPair;
	    typedef	std::map<int, Connection*>		ConnectionMap;
	    typedef	std::queue<int>					ConnectionQueue;
        
        SocketServer(std::string const& hostname, int service, bool verbose);
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
     private:
        static bool isRunning;
        bool isVerbose;
        void pushFd(int fd, int events);
        void popFd(int fd);
    protected:
        std::string                             hostname;
        int                                     service;
        
        struct sockaddr_in                      addr;
        socklen_t                               addrsize;
        ConnectionMap		                    fdConnectionMap;
	    ConnectionQueue		                    disconnectedFds;

        std::vector<pollfd>                     pollFds;
        int                                     timeout;



};