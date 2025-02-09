# Program name
NAME 		=	computor

# Project directories
S_DIR 		=	./srcs
I_DIR 		=	./include
O_DIR 		=	./obj

# Compiler and flags
CC 			=	gcc
CFLAGS 		=	-Wall -Wextra -Werror
LEAKS 		=	-fsanitize=address -g
LDFLAGS 	=	-lreadline
DEPFLAGS 	=	-MMD -MP  # Flags to generate dependency files
MLXFLAG		=	-Lusr/lib -Lmlx -lmlx -lXext -lX11 -lm -lbsd -Imlx

# Source files
SRCS		=	./srcs/algebra.c \
				./srcs/calc.c \
				./srcs/matrix.c \
				./srcs/compute.c \
				./srcs/delete.c \
				./srcs/equations.c \
				./srcs/help.c \
				./srcs/list.c \
				./srcs/main.c \
				./srcs/maths.c \
				./srcs/parse.c \
				./srcs/plot.c \
				./srcs/solve.c \
				./srcs/splash.c \
				./srcs/syntax.c \
				./srcs/utils.c

# Object files
OBJS 		= 	$(patsubst $(S_DIR)/%.c, $(O_DIR)/%.o, $(SRCS))

# Dependency files
DEPS 		= 	$(OBJS:.o=.d)

# Color codes for terminal output
RED 		= 	\033[0;31m
GREEN	 	= 	\033[0;32m
YELLOW 		= 	\033[0;33m
RESET 		= 	\033[0m

# Delete files
RM 			= 	rm -rf

# Default target
all			: 	$(NAME)

# Compile object files
$(O_DIR)/%.o: 	$(S_DIR)/%.c
				@mkdir -p $(dir $@)
				@printf "Compiling $(YELLOW)$<$(RESET)\r"
				@$(CC) $(CFLAGS) $(LEAKS) $(DEPFLAGS) -I$(I_DIR) -c $< -o $@
				@printf "                                                                         \r"

# Link program
$(NAME)		: 	$(OBJS)
				@echo "Compiling... $(GREEN)OK!$(RESET)"
				@echo "Linking object files... \c"
				@$(CC) $(OBJS) $(CFLAGS) $(LEAKS) $(LDFLAGS) $(MLXFLAG) -o $(NAME)
				@echo "$(GREEN)OK!$(RESET)"
				@echo "Program $(YELLOW)$(NAME)$(GREEN) created successfully !$(RESET)"

# Clean object files
clean		:
				@echo "$(RED)Deleting object files...$(RESET)\c"
				@$(RM) $(O_DIR)
				@echo "$(GREEN) all object files DELETED !$(RESET)"

# Clean everything and recompile
fclean		: 	clean
				@echo "$(RED)Deleting object and binary files...$(RESET)\c"
				@$(RM) $(NAME)
				@echo "$(GREEN)all files DELETED !$(RESET)"

# Recompile everything
re			: 	fclean all

# Include dependency files (if they exist)
-include $(DEPS)

# Phony targets
.PHONY		: 	all clean fclean re
