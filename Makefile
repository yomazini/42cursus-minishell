# --- Executable Name ---
NAME = minishell

# --- Compiler ---
CC = cc

# --- Flags (Apply to main compilation and FINAL link step) ---
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

# --- Remove Command ---
RM = rm -f

# --- Directories (Relative to this Makefile) ---
PARSER_DIR = parser
EXEC_DIR = execution
# Corrected path assuming libft is at the root level alongside parser/execution
LIBFT_DIR = execution/libft
# Or if it's really inside execution:
# LIBFT_DIR = execution/libft
INCLUDES_DIR = includes

# --- Library Archives ---
PARSER_LIB = $(PARSER_DIR)/libparser.a
EXEC_LIB = $(EXEC_DIR)/libexec.a
LIBFT_A = $(LIBFT_DIR)/libft.a

# --- Readline ---
READLINE_LIB_DIR = $(shell brew --prefix readline)/lib
READLINE_INC_DIR = $(shell brew --prefix readline)/include

# --- Main Source File ---
MAIN_SRC = minishell.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)
#!
PARSER_SRC = tokenize/tokenizer.c \
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
   			 tokenize/ft_print_tokens.c \
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
   			 cmd/ft_create_cmd_table.c \
   			 cmd/ft_cmd_new.c \
   			 cmd/ft_cmd_add_back.c \
   			 cmd/ft_cmd_clear.c \
   			 cmd/ft_redir_new.c \
   			 cmd/ft_redir_add_back.c \
   			 cmd/ft_redir_clear.c \
   			 cmd/ft_print_cmd_table.c

EXEC_SRC =      execution_part/find_path.c \
            execution_part/env_init.c \
            built_in/cd_built_in.c \
            execution_part/execute_builtin.c \
            execution_part/tools1.c \
            execution_part/tools2.c \
            execution_part/redirections.c \
            execution_part/tools3.c \
            execution_part/pipeline.c \
            execution_part/pipeline_utils.c \
            execution_part/executor.c \
            execution_part/executor_utils.c \
            execution_part/tools4.c \
            execution_part/tools5.c \
            built_in/env_built_in.c \
            built_in/echo_built_in.c \
            built_in/cd_built_in_utils.c \
            built_in/cd_built_in_utils2.c \
            built_in/export_built_in_utils.c \
            built_in/export_built_in_utils2.c \
            built_in/exit_built_in.c \
            built_in/export_built_in.c \
            built_in/unset_built_in.c

LIBFT_SRC =	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c	\
		ft_strlen.c ft_strchr.c ft_strrchr.c ft_strnstr.c ft_strncmp.c ft_strlcpy.c ft_strlcat.c	\
		ft_bzero.c ft_memset.c ft_memcpy.c ft_memchr.c ft_memcmp.c ft_memmove.c	\
		ft_atoi.c ft_calloc.c ft_strdup.c	\
		ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_strmapi.c ft_striteri.c ft_itoa.c	\

PARSER_OBJ = $(PARSER_SRC:.c=.o)
EXEC_OBJ = $(EXEC_SRC:.c=.o)
LIBFT_OBJ = $(LIBFT_SRC:.c=.o)

#!
# --- Include Paths (Needed ONLY for compiling MAIN_SRC) ---
INCLUDES = -I$(INCLUDES_DIR)
INCLUDES += -I$(PARSER_DIR)
INCLUDES += -I$(EXEC_DIR)
INCLUDES += -I$(LIBFT_DIR)
INCLUDES += -I$(READLINE_INC_DIR)

# --- Linker Flags ---
LDFLAGS = -L$(PARSER_DIR)
LDFLAGS += -L$(EXEC_DIR)
LDFLAGS += -L$(LIBFT_DIR)
LDFLAGS += -L$(READLINE_LIB_DIR)

# --- Libraries to Link ---
LIBS = -lexec -lparser -lft -lreadline

# --- Header Dependency ---
MAIN_HEADER = $(INCLUDES_DIR)/minishell.h

# ============================================================================ #
# --- TARGETS ---                                                              #
# ============================================================================ #

all: $(NAME)

$(NAME): $(MAIN_OBJ) $(EXEC_LIB) $(PARSER_LIB) $(LIBFT_A)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(MAIN_OBJ) $(LDFLAGS) $(LIBS) -o $(NAME) # Added @
	@echo "$(NAME) created successfully."

$(PARSER_LIB):
	@echo "Making Parser Library..."
	@$(MAKE) -C $(PARSER_DIR)

$(EXEC_LIB):
	@echo "Making Execution Library..."
	@$(MAKE) -C $(EXEC_DIR)

$(LIBFT_A):
	@echo "Making Libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(MAIN_OBJ): $(MAIN_SRC) $(MAIN_HEADER) Makefile
	@echo "Compiling $(MAIN_SRC)..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $(MAIN_SRC) -o $(MAIN_OBJ) # Added @

clean:
	@echo "Cleaning main object file..."
	@$(RM) $(MAIN_OBJ) # Added @
	@echo "Cleaning sub-directories..."
	@$(MAKE) clean -C $(PARSER_DIR)
	@$(MAKE) clean -C $(EXEC_DIR)
	@$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	@echo "Full cleaning sub-directories..."
	@$(MAKE) fclean -C $(PARSER_DIR)
	@$(MAKE) fclean -C $(EXEC_DIR)
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "Cleaning executable $(NAME)..."
	@$(RM) $(NAME) # Added @

re: fclean all

.PHONY: all clean fclean re