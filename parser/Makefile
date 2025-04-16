SRC = parser.c \
	  tokenize/ft_tokenize.c \
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


OBJS = $(SRC:.c=.o)
READLINE_LIB_DIR = $(shell brew --prefix readline)/lib
READLINE_INC_DIR = $(shell brew --prefix readline)/include

NAME = parsi
CC = cc 
CFLAGS = -Wall -Wextra -Werror -I$(READLINE_INC_DIR) 
LDFLAG = -L$(READLINE_LIB_DIR) -lreadline -lncurses
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS) parser.h
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAG)


%.o:%.c parser.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

run:
	@$(MAKE) all
	@$(MAKE) clean
	@echo "Build complete, object files cleand."
	