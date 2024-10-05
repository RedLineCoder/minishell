NAME = minishell
HEADERS = minishell.h
PARSER_PATH = utils/parser/
PARSER = $(PARSER_PATH)init_list.c $(PARSER_PATH)tokenizer.c $(PARSER_PATH)lexer.c
EXECUTOR_PATH = utils/executor/
EXECUTOR = $(EXECUTOR_PATH)utils.c $(EXECUTOR_PATH)executes.c 
SOURCES = $(PARSER) $(EXECUTOR) main.c utils/user.c utils/string_utils.c helpers/parser.c helpers/executor.c utils/environment.c helpers/environment.c
BUILTINS = builtins/cd.c builtins/exit.c builtins/pwd.c builtins/echo.c builtins/export.c builtins/unset.c builtins/env.c

READLINE = lib/readline/lib/libreadline.a
RL_FLAGS = -L${PWD}/lib/readline/lib -I${PWD}/lib/readline/include/readline -lreadline

LIBFT = lib/libft/libft.a
LIBFT_PATH = lib/libft

GNL = lib/gnl/gnl.a
GNL_PATH = lib/gnl

CC = cc
CFLAGS= -Wall -Wextra -Werror -g -fsanitize=address

all: $(NAME)

$(NAME): $(READLINE) $(LIBFT) $(GNL) $(SOURCES) $(HEADERS) $(BUILTINS)
		$(CC) $(LIBFT) $(GNL) $(CFLAGS) $(SOURCES) $(BUILTINS) $(RL_FLAGS) -o $(NAME)

$(READLINE):
		curl -O https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
		tar -xvf readline-8.2.tar.gz
		cd readline-8.2 && ./configure --prefix=${PWD}/lib/readline && make install
		$(RM) -r readline-8.2.tar.gz readline-8.2

$(LIBFT): 
		make -C $(LIBFT_PATH) all bonus

$(GNL):
		make -C $(GNL_PATH)

clean:
		$(RM) $(NAME)
		make -C $(LIBFT_PATH) clean 
		make -C $(GNL_PATH) clean

fclean: clean
		make -C $(LIBFT_PATH) fclean
		make -C $(GNL_PATH) fclean

test: all
		bash test.bash

re: fclean all
