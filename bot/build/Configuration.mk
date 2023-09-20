# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Configuration.mk                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/13 00:47:20 by mbouthai          #+#    #+#              #
#    Updated: 2023/09/20 16:32:41 by mbouthai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

define PROJECT_HEADER

   __ _     _          ____        _   
  / _| |   (_)        |  _ \      | |  
 | |_| |_   _ _ __ ___| |_) | ___ | |_ 
 |  _| __| | | '__/ __|  _ < / _ \| __|
 | | | |_  | | | | (__| |_) | (_) | |_ 
 |_|  \__| |_|_|  \___|____/ \___/ \__|
       ______                          
      |______|                         
      
endef
export PROJECT_HEADER

CXX	:= c++
CPPFLAGS	:= -Wall -Werror -Wextra -std=c++98 $(addprefix -I,$(HEADERS_DIR)) -fsanitize=address

LDFLAGS	:= -fsanitize=address

CLEAN_DIR	:= rm -fd

CLEAN_FILES	:= rm -f
