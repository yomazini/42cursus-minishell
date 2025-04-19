
READLINE_LIB_DIR = $(shell brew --prefix readline)/lib
READLINE_INC_DIR = $(shell brew --prefix readline)/include

NAME = minishell_exec_test
CC = cc 
#CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
CFLAGS = -fsanitize=address -g3

INCLUDES = -I. -I$(LIBFT_DIR)
INCLUDES += -I$(READLINE_INC_DIR)

LDFLAGS = -L$(LIBFT_DIR)
LDFLAGS += -L$(READLINE_LIB_DIR)

LIBS = -lft -lreadline

RM = rm -f

LIBFT_DIR = libft
BUILTIN_DIR = built_in
EXEC_DIR = execution_part

SRCS = main.c $(BUILTIN_DIR)/echo_built_in.c $(BUILTIN_DIR)/cd_built_in.c  $(BUILTIN_DIR)/env_built_in.c    $(BUILTIN_DIR)/exit_built_in.c    $(BUILTIN_DIR)/export_built_in.c    $(BUILTIN_DIR)/unset_built_in.c   $(BUILTIN_DIR)/pwd_built_in.c $(EXEC_DIR)/tools1.c $(EXEC_DIR)/tools2.c $(EXEC_DIR)/tools3.c $(EXEC_DIR)/tools4.c $(EXEC_DIR)/tools5.c $(EXEC_DIR)/execute_builtin.c

OBJS = $(SRCS:.c=.o)

LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $(NAME)

$(LIBFT_A):
	@make -C $(LIBFT_DIR)

%.o: %.c exec_header.h Makefile
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re sanitize