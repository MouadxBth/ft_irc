/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:13:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/13 19:00:04 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

#include "Server.hpp"
#include "User.hpp"
#include "utils.hpp"

class Command
{

private:
    std::string _name;
    std::string _description;
    int  _maxArguments;
    bool    _authRequired;
    bool    _requireTrail;
    Server  *_server;

public:
    Command();
    virtual ~Command();
    Command(const Command& instance);
    Command& operator=(const Command& instance);

    Command(const std::string& name,
        const std::string& description,
        int maxArgs,
        bool authRequired,
        bool trail);

    virtual void executeCommand(User *user, Data &data) = 0;

    const std::string&  getName() const;
    const std::string&  getDescription() const;
    int  getMaxArguments() const;
    bool  isAuthRequired() const;
    bool    requiresTrail() const;
    
    Server  *getServer() const;
    
    void    setName(const std::string& name);
    void    setDescription(const std::string& description);
    void    setMaxArguments(int maxArguments);
    void    setAuthRequired(bool auth);

    void    setRequireTrail(bool trail);

    void    setServer(Server *server);
};
