# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Configuration.mk                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/13 00:47:20 by mbouthai          #+#    #+#              #
#    Updated: 2023/07/19 02:29:06 by mbouthai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

define PROJECT_HEADER

   __ _     _          
  / _| |   (_)         
 | |_| |_   _ _ __ ___ 
 |  _| __| | | '__/ __|
 | | | |_  | | | | (__ 
 |_|  \__| |_|_|  \___|
       ______          
      |______|         

endef
export PROJECT_HEADER

CXX	:= c++
CPPFLAGS	:= -Wall -Werror -Wextra -std=c++98 -I$(HEADERS_DIR)/ #-fsanitize=address

LDFLAGS	:= #-fsanitize=address

CLEAN_DIR	:= rm -fd

CLEAN_FILES	:= rm -f
