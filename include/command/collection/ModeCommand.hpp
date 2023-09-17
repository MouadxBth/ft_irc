/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/16 18:41:51 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"

class ModeCommand : public Command
{

public:
    ModeCommand();
    ~ModeCommand();
    ModeCommand(const ModeCommand& instance);
    ModeCommand& operator=(const ModeCommand& instance);

    void executeCommand(User *user, Data &data);
};