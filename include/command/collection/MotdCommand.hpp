/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MotdCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 03:38:50 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"

class MotdCommand : public Command
{

public:
    MotdCommand();
    ~MotdCommand();
    MotdCommand(const MotdCommand& instance);
    MotdCommand& operator=(const MotdCommand& instance);

    void executeCommand(User *user, Data &data);
};