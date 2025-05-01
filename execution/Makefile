READLINE_LIB_DIR = $(shell brew --prefix readline)/lib
READLINE_INC_DIR = $(shell brew --prefix readline)/include

# Executable Name
NAME = minishell_testing

# Compiler and Base Flags
CC = cc
# CFLAGS = -Wall -Wextra -Werror -g3 # Recommend using these for development
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3 # Your current flags

# Add sanitize flags for the 'sanitize' target
SANITIZE_FLAGS = -fsanitize=address

# Remove command
RM = rm -f

# --- Directories ---
# Relative to *this* Makefile (in execution/)
LIBFT_DIR = libft
BUILTIN_DIR = built_in
EXEC_PART_DIR = execution_part
PARSER_ROOT_DIR = ../parser

# Include Paths (Relative to this Makefile)
INCLUDES = -I.                       # Current dir (for exec_header.h)
INCLUDES += -I$(LIBFT_DIR)           # Libft headers
INCLUDES += -I$(READLINE_INC_DIR)    # Readline headers
INCLUDES += -I$(PARSER_ROOT_DIR)     # Include parser directory for parser.h

# Library Paths (Relative to this Makefile)
LDFLAGS = -L$(LIBFT_DIR)
LDFLAGS += -L$(READLINE_LIB_DIR)

# Libraries to Link
LIBS = -lft -lreadline

# --- Source Files ---

# YOUR Execution Sources (Paths relative to this Makefile)
#$(EXEC_PART_DIR)/main_pipe_test.c \

EXEC_SRCS =  main.c \
            $(BUILTIN_DIR)/echo_built_in.c \
             $(BUILTIN_DIR)/env_built_in.c \
            $(EXEC_PART_DIR)/temp_converter.c \
             $(EXEC_PART_DIR)/find_path.c \
             $(EXEC_PART_DIR)/env_init.c \
             $(BUILTIN_DIR)/cd_built_in.c \
            $(EXEC_PART_DIR)/execute_builtin.c\
            $(EXEC_PART_DIR)/tools1.c \
             $(EXEC_PART_DIR)/tools2.c \
             $(EXEC_PART_DIR)/redirections.c \
             $(EXEC_PART_DIR)/tools3.c \
             $(EXEC_PART_DIR)/pipeline.c \
             $(EXEC_PART_DIR)/executor.c \
             $(EXEC_PART_DIR)/tools4.c \
             $(EXEC_PART_DIR)/tools5.c \
            $(BUILTIN_DIR)/pwd_built_in.c \
            $(BUILTIN_DIR)/exit_built_in.c \
             $(BUILTIN_DIR)/export_built_in.c \
             $(BUILTIN_DIR)/unset_built_in.c \

# MEHDI'S Parsing Sources (Paths relative to this Makefile)
#PARSE_SRCS = $(PARSER_ROOT_DIR)/tokenize/tokenizer.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_split_tokens.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_utils_split.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_isspace.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_strncpy.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_strlen.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_isquot.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_token_new.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_token_add_back.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_token_delone.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_token_clear.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_synax_error_free.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_utils_error.c \
             $(PARSER_ROOT_DIR)/tokenize/ft_print_tokens.c \
             $(PARSER_ROOT_DIR)/expander/expander.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_getenv.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_tenv_new.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_tenv_add_back.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_tenv_last.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_tenv_delone.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_tenv_clear.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_strdup.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_strchr.c \
             $(PARSER_ROOT_DIR)/expander/env/ft_strnstr.c


PARSE_SRCS = $(PARSER_ROOT_DIR)/tokenize/tokenizer.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_split_tokens.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_utils_split.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_isspace.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_strncpy.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_strlen.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_isquot.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_token_new.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_token_add_back.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_token_delone.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_token_clear.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_synax_error_free.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_utils_error.c \
	  $(PARSER_ROOT_DIR)/tokenize/ft_print_tokens.c \
	  $(PARSER_ROOT_DIR)/expander/expander.c \
	  $(PARSER_ROOT_DIR)/expander/ft_isdigit.c \
	  $(PARSER_ROOT_DIR)/expander/ft_strcmp.c \
	  $(PARSER_ROOT_DIR)/expander/ft_strjoined.c \
	  $(PARSER_ROOT_DIR)/expander/ft_substr.c \
	  $(PARSER_ROOT_DIR)/expander/ft_clean_up.c \
	  $(PARSER_ROOT_DIR)/expander/ft_expand_tools.c \
	  $(PARSER_ROOT_DIR)/expander/ft_build_variable_name.c \
	  $(PARSER_ROOT_DIR)/expander/ft_isvariablet_exist.c \
	  $(PARSER_ROOT_DIR)/expander/ft_exec_exp.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_getenv.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_tenv_new.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_tenv_add_back.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_tenv_last.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_tenv_delone.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_tenv_clear.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_strdup.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_strchr.c \
	  $(PARSER_ROOT_DIR)/expander/env/ft_strnstr.c \
	  $(PARSER_ROOT_DIR)/cmd/ft_create_cmd_table.c \
	  $(PARSER_ROOT_DIR)/cmd/ft_cmd_new.c \
	  $(PARSER_ROOT_DIR)/cmd/ft_cmd_add_back.c \
	  $(PARSER_ROOT_DIR)/cmd/ft_cmd_clear.c \
	  $(PARSER_ROOT_DIR)/cmd/ft_redir_new.c \
	  $(PARSER_ROOT_DIR)/cmd/ft_redir_add_back.c \
	  $(PARSER_ROOT_DIR)/cmd/ft_redir_clear.c \
	  $(PARSER_ROOT_DIR)/cmd/ft_print_cmd_table.c \

	  
# Combine source lists
SRCS = $(EXEC_SRCS) $(PARSE_SRCS)

# Generate object file list from source list
OBJS = $(SRCS:.c=.o)

# Libft Archive Path
LIBFT_A = $(LIBFT_DIR)/libft.a

# --- Targets ---

# Default rule
all: $(NAME)

# Link the executable
$(NAME): $(OBJS) $(LIBFT_A)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $(NAME)
	@echo "$(NAME) created."

# Build libft using its own Makefile
$(LIBFT_A):
	@echo "Making Libft..."
	@make -C $(LIBFT_DIR) --no-print-directory

# Compile source files into object files
# This generic rule handles files in different directories because OBJS contains the paths
%.o: %.c exec_header.h $(PARSER_ROOT_DIR)/parser.h Makefile
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean object files
clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJS)
	@echo "Cleaning Libft..."
	@make clean -C $(LIBFT_DIR) --no-print-directory

# Full clean (objects + executable + libft clean)
fclean: clean
	@echo "Cleaning executable..."
	@$(RM) $(NAME)
	@echo "Full cleaning Libft..."
	@make fclean -C $(LIBFT_DIR) --no-print-directory

# Rebuild everything
re: fclean all

# Sanitizer build target (compiles *with* sanitize + your base CFLAGS)
sanitize: CFLAGS += $(SANITIZE_FLAGS)
sanitize: re

.PHONY: all clean fclean re sanitize

  