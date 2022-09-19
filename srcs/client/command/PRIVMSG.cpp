/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emenella <emenella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 11:04:28 by emenella          #+#    #+#             */
/*   Updated: 2022/09/19 18:20:10 by emenella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/command/PRIVMSG.hpp"
#include "irc/Server.hpp"

PRIVMSG::PRIVMSG(Server *serv): AuthenticationCommand(serv)
{

}

PRIVMSG::PRIVMSG(PRIVMSG const& src): AuthenticationCommand(src)
{
    
}

PRIVMSG::~PRIVMSG()
{

}

int PRIVMSG::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
{
    int ret = AuthenticationCommand::execute(clicli, begin, end);
    if (ret == 1)
    {
        std::size_t len = std::distance(begin, end);
        if (len > 2)
        {
            std::string str = begin[1];
            std::string msg;
            std::vector<std::string> dest;
            size_t i = 0;
	        size_t pos;

	        while (pos = str.find(',', i), pos != std::string::npos)
	        {
		        dest.push_back(str.substr(i, pos - i));
	        	i = pos + 1;
	        }
            dest.push_back(str.substr(i, str.length() - i));
            for (args_t::iterator it = begin + 2; it != end; it++)
                msg += *it + " ";
            msg.pop_back();
            msg.erase(0, 1);
            
            for (args_t::iterator it = dest.begin(); it != dest.end(); it++)
            {
                if ((*it)[0] == '#')
                {
                    Channel* channel = _serv->findChannel(*it);
					if (channel != NULL)
					{
						if (channel->isClient(&clicli))
							channel->message(clicli, PRIVMSG_MESSAGE(clicli.getNickname(), clicli.getUsername(), clicli.getAddr(), channel->getName(), msg));
						else
							clicli << ERR_NOTONCHANNEL(*it);
					}
					else
						clicli << ERR_NOSUCHCHANNEL(clicli.getNickname(), *it);
				}
                else
                {
                    Client* client = _serv->findClient(*it);
                    if (client != NULL)
                        *client << PRIVMSG_MESSAGE(clicli.getNickname(), clicli.getUsername(), clicli.getAddr(), client->getNickname(), msg);
                    else
                        clicli << ERR_NOSUCHNICK(*it);
                }
            }
            
        }
    }
    return 0;
}
