/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NoticeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/12 09:57:20 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"

class NoticeCommand : public Command
{

public:
    NoticeCommand();
    ~NoticeCommand();
    NoticeCommand(const NoticeCommand& instance);
    NoticeCommand& operator=(const NoticeCommand& instance);

    void executeCommand(User *user, Data &data);
};