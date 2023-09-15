/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandData.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 21:54:18 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/15 13:40:53 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

class User;

typedef struct s_data
{
    std::string prefix;
    std::string command;
    std::vector<std::string> arguments;
    std::string trail;
	std::pair<bool, User *> simultaneousNickname;
	bool valid;
    bool trailPresent;
} Data;