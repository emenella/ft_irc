#include "client/command/NOTICE.hpp"
#include "irc/Server.hpp"

NOTICE::NOTICE(Server *serv): PRIVMSG(serv)
{

}

NOTICE::NOTICE(NOTICE const& src): PRIVMSG(src._serv)
{

}

NOTICE::~NOTICE()
{

}

int NOTICE::execute(Client &clicli, args_t::iterator begin, args_t::iterator end)
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
                        channel->message(clicli, NOTICE_MESSAGE(clicli.getNickname(), channel->getName(), msg));
                    else
                        clicli << ERR_NOSUCHCHANNEL(*it);
                }
                else
                {
                    Client* client = _serv->findClient(*it);
                    if (client != NULL)
                        *client << NOTICE_MESSAGE(clicli.getNickname(), client->getNickname(), msg);
                    else
                        clicli << ERR_NOSUCHNICK(*it);
                }
            }
            
        }
    }
    return 0;
}