/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:13:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 01:24:56 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

#include "User.hpp"
#include "CommandData.hpp"
#include "CommandReplies.hpp"

class Command
{

private:
    std::string _name;
    
    bool        _authRequired;
    bool        _requireTrail;

    Command();

protected:
    Command(const Command& instance);
    Command(const std::string& name,
        bool authRequired,
        bool trail);
    

public:
    virtual ~Command();
    Command& operator=(const Command& instance);

    virtual void executeCommand(User *user, Data &data) = 0;

    const std::string&  getName() const;
    
    bool                isAuthRequired() const;
    bool                requiresTrail() const;
    

    void    setName(const std::string& name);

    void    setAuthRequired(bool auth);
    void    setRequireTrail(bool trail);
};
