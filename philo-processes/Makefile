# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/25 16:04:25 by ipersids          #+#    #+#              #
#    Updated: 2025/03/05 14:08:04 by ipersids         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    RM = del /q
    RM_DIR = rmdir /s /q
else
    RM = rm -f
    RM_DIR = rm -rf
endif

# Program name
NAME			:= philo_bonus

# Compilation variables
CC				:= clang
CFLAGS			:= -Wall -Wextra -Werror -pthread
HDRS			:= -Iinclude

# Directories
SRC_DIR			:= src
OBJ_DIR			:= obj

# Sources and objects
SRCS			:=	$(SRC_DIR)/destructor.c \
					$(SRC_DIR)/init_simulation.c \
					$(SRC_DIR)/parse_argv.c \
					$(SRC_DIR)/run_simulation.c \
					$(SRC_DIR)/utils.c \
					$(SRC_DIR)/init_monitoring.c

SRC_MAIN		:= main.c
OBJS			:= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_MAIN		:= $(SRC_MAIN:%.c=$(OBJ_DIR)/%.o)

# RULES
all: $(NAME)

$(NAME): $(OBJS) $(OBJ_MAIN)
	$(CC) $(CFLAGS) $(OBJS) $(OBJ_MAIN) $(HDRS) -o $(NAME)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(HDRS) -c $< -o $@

$(OBJ_DIR)/main.o: $(SRC_MAIN)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(HDRS) -c $< -o $@

clean:
	$(RM_DIR) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: CFLAGS += -fsanitize=address -g
debug: fclean all

.PHONY: all clean fclean re debug