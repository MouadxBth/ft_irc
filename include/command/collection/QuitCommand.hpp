/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/17 16:56:40 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"

class QuitCommand : public Command
{

public:
    QuitCommand();
    ~QuitCommand();
    QuitCommand(const QuitCommand& instance);
    QuitCommand& operator=(const QuitCommand& instance);

    void executeCommand(User *user, Data &data);
};