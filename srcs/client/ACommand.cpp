/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmangin <bmangin@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 16:46:18 by bmangin           #+#    #+#             */
/*   Updated: 2022/04/28 17:42:54by bmangin          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "client/ACommand.hpp"

ACommand::ACommand(Server *serv) : serv(serv)
{
}

ACommand::ACommand(ACommand const& src)  : serv(src.serv)
{
    #ifdef DEBUG
        std::cout << "Copy Constructeur Called" << std::endl;
    #endif
}
ACommand::~ACommand()
{
    #ifdef DEBUG
        std::cout << "Destructeur Called" << std::endl;
    #endif
}

ACommand&   ACommand::operator=(ACommand const& rhs)
{
	if (this != &rhs)
	{
		this->serv = rhs.serv;
	}
	return *this;
}

const Server *				ACommand::getServ() const
{
	return this->serv;
}
