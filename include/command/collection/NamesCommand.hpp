/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NamesCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/12 09:57:20 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"

class NamesCommand : public Command
{

public:
    NamesCommand();
    ~NamesCommand();
    NamesCommand(const NamesCommand& instance);
    NamesCommand& operator=(const NamesCommand& instance);

    void executeCommand(User *user, Data &data);
};