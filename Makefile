NAME = minishell

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

LIBFT_DIR = libft
PARSER_DIR = parser
EXEC_DIR = execution
INCLUDES_DIR = includes

READLINE_PREFIX = $(shell brew --prefix readline)
READLINE_INC_DIR = $(READLINE_PREFIX)/include
READLINE_LIB_DIR = $(READLINE_PREFIX)/lib

INCLUDES = -I$(INCLUDES_DIR)
INCLUDES += -I$(PARSER_DIR)
INCLUDES += -I$(EXEC_DIR)
INCLUDES += -I$(LIBFT_DIR)
INCLUDES += -I$(READLINE_INC_DIR)

MAIN_SRC = minishell.c

LIBFT_SRC = $(addprefix $(LIBFT_DIR)/, \
		ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c \
		ft_strlen.c ft_strchr.c ft_strrchr.c ft_strnstr.c ft_strncmp.c ft_strlcpy.c ft_strlcat.c \
		ft_bzero.c ft_memset.c ft_memcpy.c ft_memchr.c ft_memcmp.c ft_memmove.c \
		ft_atoi.c ft_calloc.c ft_strdup.c \
		ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_strmapi.c ft_striteri.c ft_itoa.c \
		)

SRCS_PARSER = $(addprefix $(PARSER_DIR)/, \
		tokenize/tokenizer.c \
		tokenize/ft_split_tokens.c \
		tokenize/ft_utils_split.c \
		tokenize/ft_isspace.c \
		tokenize/ft_strncpy.c \
		tokenize/ft_isquot.c \
		tokenize/ft_token_new.c \
		tokenize/ft_token_add_back.c \
		tokenize/ft_token_delone.c \
		tokenize/ft_token_clear.c \
		tokenize/ft_synax_error_free.c \
		tokenize/ft_utils_error.c \
		expander/expander.c \
		expander/ft_strcmp.c \
		expander/ft_strjoined.c \
		expander/ft_substr.c \
		expander/ft_clean_up.c \
		expander/ft_expand_tools.c \
		expander/ft_build_variable_name.c \
		expander/ft_isvariablet_exist.c \
		expander/ft_exec_exp.c \
		expander/env/ft_getenv.c \
		expander/env/ft_tenv_new.c \
		expander/env/ft_tenv_add_back.c \
		expander/env/ft_tenv_last.c \
		expander/env/ft_tenv_delone.c \
		expander/env/ft_tenv_clear.c \
		expander/ft_utils_expand.c \
		cmd/ft_create_cmd_table.c \
		cmd/ft_cmd_new.c \
		cmd/ft_cmd_add_back.c \
		cmd/ft_cmd_clear.c \
		cmd/ft_redir_new.c \
		cmd/ft_redir_add_back.c \
		cmd/ft_redir_clear.c \
		cmd/ft_utils_cmd.c \
		)

SRCS_EXEC = $(addprefix $(EXEC_DIR)/, \
		built_in/echo_built_in.c \
		built_in/env_built_in.c \
		execution_part/find_path.c \
		execution_part/env_init.c \
		built_in/cd_built_in.c \
		execution_part/execute_builtin.c \
		execution_part/tools1.c \
		execution_part/tools2.c \
		execution_part/redirections.c \
		execution_part/tools3.c \
		built_in/cd_built_in_utils.c \
		built_in/cd_built_in_utils2.c \
		built_in/export_built_in_utils.c \
		built_in/export_built_in_utils2.c \
		execution_part/pipeline.c \
		execution_part/pipeline_utils.c \
		execution_part/executor.c \
		execution_part/executor_utils.c \
		execution_part/tools4.c \
		execution_part/tools5.c \
		built_in/pwd_built_in.c \
		built_in/exit_built_in.c \
		built_in/export_built_in.c \
		execution_part/heredoc/process_heredoc.c  \
		execution_part/heredoc/heredoc.c \
		execution_part/heredoc/heredoc_utils.c \
		built_in/unset_built_in.c \
		)

MAIN_OBJ = $(MAIN_SRC:.c=.o)
LIBFT_OBJS = $(LIBFT_SRC:.c=.o)
PARSER_OBJS = $(SRCS_PARSER:.c=.o)
EXEC_OBJS = $(SRCS_EXEC:.c=.o)

ALL_OBJS = $(MAIN_OBJ) $(LIBFT_OBJS) $(PARSER_OBJS) $(EXEC_OBJS)

HEADER_MAIN_APP = $(INCLUDES_DIR)/minishell.h
HEADER_LIBFT = $(LIBFT_DIR)/libft.h
HEADER_PARSER = $(PARSER_DIR)/parser.h
HEADER_EXEC = $(EXEC_DIR)/exec_header.h

LDFLAGS = -L$(READLINE_LIB_DIR)
LIBS_TO_LINK = -lreadline

all: $(NAME)

$(NAME): $(ALL_OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(ALL_OBJS) $(LDFLAGS) $(LIBS_TO_LINK) -o $(NAME)
	@echo "$(NAME) successfully created."

$(MAIN_OBJ): $(MAIN_SRC) $(HEADER_MAIN_APP) $(HEADER_PARSER) $(HEADER_EXEC) $(HEADER_LIBFT) Makefile
	@echo "Compiling $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_DIR)/%.o: $(LIBFT_DIR)/%.c $(HEADER_LIBFT) Makefile
	@echo "Compiling $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c $(HEADER_PARSER) $(HEADER_LIBFT) $(HEADER_MAIN_APP) Makefile
	@echo "Compiling $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXEC_DIR)/%.o: $(EXEC_DIR)/%.c $(HEADER_EXEC) $(HEADER_PARSER) $(HEADER_LIBFT) $(HEADER_MAIN_APP) Makefile
	@echo "Compiling $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@$(RM) $(ALL_OBJS)
	@echo "Object files cleaned."

fclean: clean
	@echo "Cleaning executable $(NAME)..."
	@$(RM) $(NAME)
	@echo "$(NAME) cleaned."

re: fclean all

.PHONY: all clean fclean re