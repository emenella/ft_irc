/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:28:25 by emenella          #+#    #+#             */
/*   Updated: 2022/09/08 16:10:38 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/SocketServer.hpp"

SocketServer::SocketServer(std::string const& hostname, int service, bool verbose = false): SocketListener(), isRunning(false), isVerbose(verbose), hostname(hostname), service(service), timeout(TIMEOUT)
{
    if (isVerbose)
        std::cout << "SocketServer::SocketServer()" << std::endl;
   pushFd(sock, POLLIN);
}

SocketServer::SocketServer(SocketServer const &src): SocketListener(src)
{
    if (isVerbose)
        std::cout << "SocketServer::SocketServer()" << std::endl;
    pushFd(src.sock, POLLIN);
}

SocketServer &SocketServer::operator=(SocketServer const &rhs)
{
    if (isVerbose)
        std::cout << "SocketServer::SocketServer()" << std::endl;
    SocketListener::operator=(rhs);
    return *this;
}

SocketServer::~SocketServer()
{
    popFd(sock);
    if (isVerbose)
        std::cout << "SocketServer::~SocketServer()" << std::endl;
}

void	SocketServer::onConnection(int connectionFd, sockaddr_in& address)
{
    (void)address;
    if (isVerbose)
        std::cout << "New connection from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
    pushFd(connectionFd, POLLIN | POLLHUP);
}

void	SocketServer::onDisconnection(Connection& connection)
{
    if (isVerbose)
        std::cout << "Disconnection from " << connection.getAddr()<< ":" << connection.getPort() << std::endl;
    popFd(connection.getSock());
}

void	SocketServer::onMessage(Connection& connection, std::string const& message)
{
    (void)connection;
    if (message.empty())
        return;
    if (isVerbose)
        std::cout << "Message from " << connection.getAddr() << ":" << connection.getPort() << ": " << message << std::endl;
}

void SocketServer::start()
{
    bind(this->hostname, this->service);
    listen();
    isRunning = true;
    while (isRunning)
    {
        
        poll();
        try
        {
            std::vector<pollfd>::iterator ite = pollFds.end();
            for (std::vector<pollfd>::iterator it = pollFds.begin(); it != ite; ++it)
            {
                if (it->revents & POLLHUP)
                {
                    Connection *connection = fdConnectionMap[it->fd];
                    if (connection)
                        onDisconnection(*connection);
                }
                else if (it->revents & POLLIN)
                {
                    if (it->fd == sock)
                    {
                        int connectionFd = accept(addr);
                        if (connectionFd != -1)
                        {
                            onConnection(connectionFd, addr);
                        }
                    }
                    else
                    {
                        Connection* connection = fdConnectionMap[it->fd];
                        receiveAndSend(*connection);
                    }
                }
            }
        }
        catch (SocketException const& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void SocketServer::stop()
{
    isRunning = false;
}

void SocketServer::receiveAndSend(Connection &connection)
{
    try
    {
        std::string message;
        connection >> message;
        while (message.find("\r\n") != std::string::npos)
        {
            size_t pos = message.find("\r\n");
            if (pos != std::string::npos)
            {
                onMessage(connection, message.substr(0, pos));
                message.erase(0, pos + 2);
            }
        }
    }
    catch (SocketException const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    try
    {
        connection.flush();
    }
    catch (SocketException const& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void SocketServer::pushFd(int fd, int events)
{
    if (isVerbose)
        std::cout << "SocketServer::pushFd(" << fd << ")" << std::endl;
    pollfd pollfd;
    pollfd.fd = fd;
    pollfd.events = events;
    pollFds.push_back(pollfd);
}

void SocketServer::popFd(int fd)
{
    if (isVerbose)
        std::cout << "SocketServer::popFd(" << fd << ")" << std::endl;
    for (std::vector<pollfd>::iterator it = pollFds.begin(); it != pollFds.end(); ++it)
    {
        if (it->fd == fd)
        {
            pollFds.erase(it);
            break;
        }
    }
}

void SocketServer::poll()
{
    if (isVerbose)
        std::cout << "Waiting Resquest" << std::endl;
    int ret = ::poll((pollfd *)&pollFds[0], pollFds.size(), -1);
	if (ret == -1)
        throw SocketException("poll");
}

void SocketServer::listen()
{
    SocketListener::listen();
    if (isVerbose)
        std::cout << "Listening on " << hostname << ":" << service << std::endl;
}

std::string SocketServer::getHostname() const
{
    return hostname;
}