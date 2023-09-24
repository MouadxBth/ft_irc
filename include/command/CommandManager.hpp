/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 12:00:58 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/24 12:56:05 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <map>
# include <cstdarg>

# include "Server.hpp"
# include "CommandData.hpp"

class CommandManager
{

private:
    static CommandManager  *_instance;
    std::map<std::string, Command *> _registeredCommands;

public:
    CommandManager();
    ~CommandManager();
    CommandManager(const CommandManager& instance);
    CommandManager& operator=(const CommandManager& instance);

    const std::map<std::string, Command *>& getRegisteredCommands() const;

    void    registerCommand(Command *command);

    void    registerCommands(Command *command, ...);

    void    unregisterCommand(const std::string& command);

    void    unregisterCommands(std::string *command, ...);

    void    executeCommand(User *user, Data &data);

    void    cleanUp();

    static CommandManager *getInstance();

};