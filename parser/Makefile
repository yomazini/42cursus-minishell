# --- Library Name ---
NAME = libparser.a

# --- Compiler & Archiver ---
CC = cc
AR = ar rcs
RM = rm -f

# --- Flags ---
# Flags passed down from the top-level make or defined here
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
# Handle SANITIZE variable if passed from top-level (optional redundancy)
ifdef SANITIZE
	CFLAGS += -fsanitize=address
endif

# --- Directories (Relative to this Makefile) ---
LIBFT_DIR = ../libft
INCLUDES_DIR = ../includes
# Need readline includes if parser.h or sources use them
READLINE_INC_DIR = $(shell brew --prefix readline)/include

# --- Include Paths ---
INCLUDES = -I.              # Find parser.h
INCLUDES += -I$(LIBFT_DIR)  # Find libft.h
INCLUDES += -I$(INCLUDES_DIR) # Find minishell.h (if needed directly, unlikely now)
INCLUDES += -I$(READLINE_INC_DIR) # Find readline headers

# --- Source Files (Relative to this Makefile) ---
# List ONLY parser sources, paths relative to *this* directory
SRCS =	tokenize/tokenizer.c \
		tokenize/ft_split_tokens.c \
		tokenize/ft_utils_split.c \
		tokenize/ft_isspace.c \
		tokenize/ft_strncpy.c \
		tokenize/ft_strlen.c \
		tokenize/ft_isquot.c \
		tokenize/ft_token_new.c \
		tokenize/ft_token_add_back.c \
		tokenize/ft_token_delone.c \
		tokenize/ft_token_clear.c \
		tokenize/ft_synax_error_free.c \
		tokenize/ft_utils_error.c \
		expander/expander.c \
		expander/ft_isdigit.c \
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
		expander/env/ft_strdup.c \
		expander/env/ft_strchr.c \
		expander/env/ft_strnstr.c \
		expander/ft_utils_expand.c \
		cmd/ft_create_cmd_table.c \
		cmd/ft_cmd_new.c \
		cmd/ft_cmd_add_back.c \
		cmd/ft_cmd_clear.c \
		cmd/ft_redir_new.c \
		cmd/ft_redir_add_back.c \
		cmd/ft_redir_clear.c \
		cmd/ft_utils_cmd.c \

# --- Objects ---
OBJS = $(SRCS:.c=.o)

# --- Header Dependency ---
HEADER = parser.h

# --- Rules ---
all: $(NAME)

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo "Parser Library $(NAME) created."

# Compile source files into object files
%.o: %.c $(HEADER) Makefile # Depending on parser.h and this Makefile
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re