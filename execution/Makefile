# # File: execution/Makefile

# # --- Library Name ---
# NAME = libexec.a

# # --- Compiler & Archiver ---
# CC = cc
# AR = ar rcs
# RM = rm -f

# # --- Flags ---
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
# ifdef SANITIZE
# 	CFLAGS += -fsanitize=address
# endif

# # --- Directories (Relative to this Makefile) ---
# # Assuming libft is one level up from execution/
# LIBFT_DIR = ../libft
# # Or if libft is ALSO inside execution (less common)
# # LIBFT_DIR = libft
# PARSER_DIR = ../parser
# INCLUDES_DIR = ../includes
# READLINE_INC_DIR = $(shell brew --prefix readline)/include

# # --- Include Paths ---
# INCLUDES = -I.
# INCLUDES += -I$(LIBFT_DIR)
# INCLUDES += -I$(PARSER_DIR)
# INCLUDES += -I$(INCLUDES_DIR)
# INCLUDES += -I$(READLINE_INC_DIR)

# # --- Source Files ---
# SRCS =  built_in/echo_built_in.c \
# 		built_in/env_built_in.c \
# 		execution_part/find_path.c \
# 		execution_part/env_init.c \
# 		built_in/cd_built_in.c \
# 		execution_part/execute_builtin.c \
# 		execution_part/tools1.c \
# 		execution_part/tools2.c \
# 		execution_part/redirections.c \
# 		execution_part/tools3.c \
#     	built_in/cd_built_in_utils.c \
#     	built_in/cd_built_in_utils2.c \
#         built_in/export_built_in_utils.c \
#         built_in/export_built_in_utils2.c \
# 		execution_part/pipeline.c \
# 		execution_part/pipeline_utils.c \
# 		execution_part/executor.c \
# 		execution_part/executor_utils.c \
# 		execution_part/tools4.c \
# 		execution_part/tools5.c \
# 		built_in/pwd_built_in.c \
# 		built_in/exit_built_in.c \
# 		built_in/export_built_in.c \
# 		execution_part/heredoc/process_heredocs.c \
# 		built_in/unset_built_in.c

# # --- Objects ---
# OBJS = $(SRCS:.c=.o)

# # --- Header Dependency ---
# HEADERS = exec_header.h $(PARSER_DIR)/parser.h

# # --- Rules ---
# all: $(NAME)

# $(NAME): $(OBJS)
# 	@$(AR) $(NAME) $(OBJS) # Added @

# %.o: %.c $(HEADERS) Makefile
# 	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ # Added @

# clean:
# 	@$(RM) $(OBJS) # Added @

# fclean: clean
# 	@$(RM) $(NAME) # Added @

# re: fclean all

# .PHONY: all clean fclean re