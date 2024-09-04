# Program name
NAME 		=	computor

# Project directories
SRCDIR 		=	./srcs
INCDIR 		=	./include
OBJDIR 		=	./obj

# Compiler and flags
CC 			=	gcc
CFLAGS 		=	-Wall -Wextra -Werror
LEAKS 		=	-fsanitize=address -g
LDFLAGS 	=	-lreadline

# Source files
SRCS		=	./srcs/calc.c \
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
				./srcs/types.c \
				./srcs/utils.c

ifeq ($(shell uname -s), Darwin)
	MLXFLAG = -lmlx -framework OpenGL -framework AppKit
else
	MLXFLAG	= -Lusr/lib -Lmlx_linux -lmlx -lXext -lX11 -lm -lbsd -Imlx_linux
endif

# Object files
OBJS 		= 	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

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

$(OBJDIR)/%.o: 	$(SRCDIR)/%.c $(INCDIR)
				@mkdir -p $(dir $@)
				@printf "Compiling $(YELLOW)$<$(RESET)\r"
				@$(CC) $(CFLAGS) $(LEAKS) -I$(INCDIR) -c $< -o $@
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
				@$(RM) $(OBJDIR)
				@echo "$(GREEN) all object files DELETED !$(RESET)"

# Clean everything and recompile
fclean		: 	clean
				@echo "$(RED)Deleting object and binary files...$(RESET)\c"
				@$(RM) $(NAME)
				@echo "$(GREEN)all files DELETED !$(RESET)"

# Recompile everything
re			: 	fclean all

# Prevent errors if object files are deleted
-include $(OBJS:.o=.d)

# Phony targets
.PHONY		: 	all clean fclean re
