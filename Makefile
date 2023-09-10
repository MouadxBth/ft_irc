# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/12 23:26:34 by mbouthai          #+#    #+#              #
#    Updated: 2023/08/19 11:22:10 by mbouthai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include build/Files.mk
include build/Utilities.mk
include build/Configuration.mk

NAME	= ircserv

all: $(NAME)

debug: fclean
debug: CPPFLAGS += -g

debug: all

$(NAME): $(OBJECTS)
	$(call colorize,$(GREEN),Building Project...)
	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)
	$(call colorize,$(BOLD),$$PROJECT_HEADER)

$(OBJS_DIR):
	$(call colorize,$(GREEN),Creating Objects Directory...)
	-mkdir $@
	-mkdir $(DIRS)

$(DEPS_DIR):
	$(call colorize,$(GREEN),Creating Dependencies Directory...)
	-mkdir $@

compile:
	$(call colorize,$(GREEN),Compiling...)

$(OBJECTS): | $(OBJS_DIR)

$(OBJECTS): | $(DEPS_DIR)

$(OBJECTS): | compile

$(OBJECTS): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%
	$(CXX) $(CPPFLAGS) -MMD -MP -MF $(addprefix $(DEPS_DIR)/,$(notdir $(<:%=%.d))) -c $< -o $@

cleanobjs:
	$(call colorize,$(GREEN),Cleaning Object Files...)
	-$(CLEAN_FILES) $(wildcard $(OBJECTS))

cleanobjdir: cleanobjs
	$(call colorize,$(GREEN),Cleaning Objects Directory...)
	-$(CLEAN_DIR) $(DIRS)
	-$(CLEAN_DIR) $(OBJS_DIR)

cleandeps:
	$(call colorize,$(GREEN),Cleaning Dependency Files...)
	-$(CLEAN_FILES) $(wildcard $(DEPENDENCIES))

cleandepdir: cleandeps
	$(call colorize,$(GREEN),Cleaning Dependencies Directory...)
	-$(CLEAN_DIR) $(DEPS_DIR)

cleanalert:
	$(call colorize,$(GREEN),Cleaning Up...)

clean: cleanalert cleanobjdir cleandepdir

fclean: clean
	$(call colorize,$(GREEN),Cleaning Executable...)
	-$(CLEAN_FILES) $(NAME)

re: fclean all

-include $(DEPENDENCIES)
	
.phony: all clean fclean re
