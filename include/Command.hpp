/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:13:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/08/19 11:15:46 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

class Command
{

private:
    std::string name;
    std::string description;
    size_t  code;

public:
    Command();
    ~Command();
    Command(const Command& instance);
    Command& operator=(const Command& instance);
};
