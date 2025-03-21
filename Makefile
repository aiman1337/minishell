NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra
LIB_RL = -L/goinfre/ahouass/homebrew/opt/readline/lib -lreadline
INCLUDE_RL = -I/goinfre/ahouass/homebrew/opt/readline/include

SRC = minishell.c tokenizer.c ft_tree.c \
		built_in/cd.c built_in/export.c built_in/env.c built_in/unset.c  built_in/exit.c built_in/pwd.c built_in/echo.c \
		str_utils/str_utils1.c str_utils/str_utils2.c str_utils/str_utils3.c str_utils/ft_print.c \
		utils/utils.c utils/free.c utils/errors.c utils/ft_split.c utils/get_next_line.c \
		execution/exec.c 

OBJ = $(SRC:.c=.o)


$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB_RL) -o $(NAME)

all: $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) $(INCLUDE_RL) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 
