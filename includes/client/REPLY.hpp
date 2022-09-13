#pragma once

#define RPL_WELCOME(nick, user, host) "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host
#define RPL_AWAY(sender, msg) "301 " + sender + " :" + msg
#define RPL_TOPIC(client, channel, topic) "332 " + client + " " + channel + " :" + topic
#define RPL_NAMREPLY(nick, channel, listUsers) "353 " + nick + " = " + channel + " :" + listUsers
#define RPL_ENDOFNAMES(nick, channel) "366 " + nick + " " + channel + " :End of NAMES list"

#define PRIVMSG_MESSAGE(sender, receiver, message) 
#define JOIN_MESSAGE(nick, userName, userIP, channel) ":" + nick + "!" + userName + "@" + userIP + " JOIN " + channel
#define PART_MESSAGE(nick, userName, userIP, channel) ":" + nick + "!" + userName + "@" + userIP + " PART " + channel

#define ERR_NOSUCHNICK(nick) "401 " nick " :No such nick/channel"
#define ERR_BADCARCHAN(chan) "432 JOIN " + chan + " :Bad characters"
#define ERR_NOSUCHCHANNEL(channel) "403 " + channel + " :No such channel"
#define ERR_NOTONCHANNEL(channel) "442 " + channel + " :You're not on that channel"
#define ERR_CANNOTSENDTOCHAN(channel) "404 " channel " :Cannot send to channel"
#define ERR_UNKNOWNCOMMAND(command) "421 "  command  " :Unknown command"
#define ERR_NONICKNAMEGIVEN "431 :No nickname given"
#define ERR_ERRONEUSNICKNAME "432 :Erroneus nickname"
#define ERR_NICKNAMEINUSE "433 :Nickname is already in use"
#define ERR_NICKCOLLISION(nick, user, host) "436 "  "<nick> :Nickname collision KILL from <user>@<host>"
#define ERR_NOTREGISTERED "451 :You have not registered"
#define ERR_NEEDMOREPARAMS(command) "461 " command " :Not enough parameters"
#define ERR_ALREADYREGISTRED "462 :You have already registered"