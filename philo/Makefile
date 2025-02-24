# Platform-specific settings
ifeq ($(OS),Windows_NT)
    RM = del /q
    RM_DIR = rmdir /s /q
else
    RM = rm -f
    RM_DIR = rm -rf
endif

# Program name
NAME			:= philo

# Compilation variables
CC				:= clang
CFLAGS			:= -Wall -Wextra -Werror -pthread
HDRS			:= -Iinclude

# Directories
SRC_DIR			:= src
OBJ_DIR			:= obj

# Sources and objects
SRCS			:= $(SRC_DIR)/philo_argv_check.c \
				   $(SRC_DIR)/philo_destroy.c \
				   $(SRC_DIR)/philo_exit.c \
				   $(SRC_DIR)/philo_struct_init.c \
				   $(SRC_DIR)/philo_pthread_create.c \
				   $(SRC_DIR)/philo_routine_start.c \
				   $(SRC_DIR)/philo_routine_actions.c \
				   $(SRC_DIR)/philo_routine_check.c \
				   $(SRC_DIR)/philo_time_manager.c \
				   $(SRC_DIR)/philo_print_message.c

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

debug:
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "SRC_MAIN: $(SRC_MAIN)"
	@echo "OBJ_MAIN: $(OBJ_MAIN)"

.PHONY: all clean fclean re debug