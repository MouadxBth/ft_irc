# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Files.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/12 23:45:12 by mbouthai          #+#    #+#              #
#    Updated: 2023/09/11 18:38:14 by mbouthai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Headers/Includes

HEADERS_DIR	:= include

#HEADERS	=
#HEADERS	:= $(addprefix $(HEADERS_DIR)/,$(HEADERS))

# Sources

SRCS_DIR	:= srcs

#COLOR_DIR	:= color
#COLOR_SRCS	= color_utils.c color.c
#COLOR_SRCS	:= $(addprefix $(COLOR_DIR)/,$(COLOR_SRCS))

SOURCES	= User.cpp Channel.cpp utils.cpp Server.cpp ServerUser.cpp main.cpp \
	#$(COLOR_SRCS) \
	

SOURCES	:= $(addprefix $(SRCS_DIR)/,$(SOURCES))

# Objects/Output files

OBJS_DIR	= obj
#OBJECTS	:= $(patsubst %,%.o,$(addprefix $(OBJS_DIR)/,$(notdir $(SOURCES))))
OBJECTS	:= $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%.o,$(SOURCES))

# Dependencies

DEPS_DIR	= dep
DEPENDENCIES	:= $(patsubst %,%.d,$(addprefix $(DEPS_DIR)/,$(notdir $(SOURCES))))

DIRS	= #$(COLOR_DIR) 
DIRS	:= $(addprefix $(OBJS_DIR)/,$(DIRS))
