# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Files.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/12 23:45:12 by mbouthai          #+#    #+#              #
#    Updated: 2023/10/03 21:16:39 by mbouthai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Sources

SRCS_DIR	:= srcs

CHANNEL_DIR	:= channel
CHANNEL_SRCS	= Channel.cpp
CHANNEL_SRCS	:= $(addprefix $(CHANNEL_DIR)/,$(CHANNEL_SRCS))

COMMAND_DIR	:= command
COMMANDS_DIR	:= commands
COMMANDS_SRCS	= NickCommand.cpp PassCommand.cpp \
	UserCommand.cpp PrivMsg.cpp \
	InviteCommand.cpp JoinCommand.cpp \
	TopicCommand.cpp KickCommand.cpp \
	PartCommand.cpp QuitCommand.cpp \
	MotdCommand.cpp NamesCommand.cpp \
	NoticeCommand.cpp ModeCommand.cpp \
	ListCommand.cpp

COMMAND_SRCS	= $(addprefix $(COMMANDS_DIR)/,$(COMMANDS_SRCS)) Command.cpp CommandManager.cpp
COMMAND_SRCS	:= $(addprefix $(COMMAND_DIR)/,$(COMMAND_SRCS))

SERVER_DIR	:= server
SERVER_SRCS	= Server.cpp ServerConfiguration.cpp ServerManagement.cpp \
	ServerConnection.cpp ServerHandler.cpp ServerCleanup.cpp
SERVER_SRCS	:= $(addprefix $(SERVER_DIR)/,$(SERVER_SRCS))

USER_DIR	:= user
USER_SRCS	= User.cpp
USER_SRCS	:= $(addprefix $(USER_DIR)/,$(USER_SRCS))

UTILS_DIR	:= utilities
UTILS_SRCS	= Utilities.cpp
UTILS_SRCS	:= $(addprefix $(UTILS_DIR)/,$(UTILS_SRCS))

SOURCES	= main.cpp \
	$(SERVER_SRCS) \
	$(CHANNEL_SRCS) \
	$(USER_SRCS) \
	$(UTILS_SRCS) \
	$(COMMAND_SRCS) 
	

SOURCES	:= $(addprefix $(SRCS_DIR)/,$(SOURCES))

# Headers/Includes

HEADER_DIR	:= include

HEADER_DIRS	:= channel command command/collection user utilities

HEADERS_DIR	:= $(addprefix $(HEADER_DIR)/,$(HEADER_DIRS)) $(HEADER_DIR)


#HEADERS	=
#HEADERS	:= $(addprefix $(HEADERS_DIR)/,$(HEADERS))

# Objects/Output files

OBJS_DIR	= obj
#OBJECTS	:= $(patsubst %,%.o,$(addprefix $(OBJS_DIR)/,$(notdir $(SOURCES))))
OBJECTS	:= $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%.o,$(SOURCES))

# Dependencies

DEPS_DIR	= dep
DEPENDENCIES	:= $(patsubst %,%.d,$(addprefix $(DEPS_DIR)/,$(notdir $(SOURCES))))

DIRS	= $(SERVER_DIR) \
	$(CHANNEL_DIR) \
	$(USER_DIR) \
	$(UTILS_DIR) \
	$(COMMAND_DIR)/$(COMMANDS_DIR) \
	$(COMMAND_DIR) \

DIRS	:= $(addprefix $(OBJS_DIR)/,$(DIRS))