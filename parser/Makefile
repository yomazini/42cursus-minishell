SRC = parser.c \
	  tokenize/tokenizer.c \
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



OBJS = $(SRC:.c=.o)

NAME = parsi
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
LDFLAG = -lreadline -lncurses
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS) parser.h
	@echo "Linking $(NAME) ..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAG)
	@echo "parsi Successfully built."

%.o:%.c parser.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@${RM} ${OBJS}
	@echo "Object files cleaned"

fclean: clean
	@${RM} ${NAME}
	@echo "Excutable $(NAME) cleaned."

re: fclean all
	@echo "Rebuilt $(NAME)."

run:
	@$(MAKE) --no-print-directory all
	@$(MAKE) --no-print-directory clean
	@echo "Build complete, object files cleand."
	