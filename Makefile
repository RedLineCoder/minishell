NAME = minishell
HEADERS = minishell.h
SOURCES = main.c

READLINE = lib/readline/lib/libreadline.a
RL_FLAGS = -L${PWD}/lib/readline/lib -I${PWD}/lib/readline/include/readline -lreadline

LIBFT = lib/libft/libft.a
LIBFT_PATH = lib/libft

CC = cc
CFLAGS= -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(READLINE) $(LIBFT) $(SOURCES) $(HEADERS)
	$(CC) $(RL_FLAGS) $(LIBFT) $(CFLAGS) $(SOURCES) -o $(NAME)

$(READLINE):
	curl -O https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
	tar -xvf readline-8.2.tar.gz
	cd readline-8.2 && ./configure --prefix=${PWD}/lib/readline && make install
	$(RM) -r readline-8.2.tar.gz readline-8.2

$(LIBFT): 
	make -C $(LIBFT_PATH) all bonus

clean:
	$(RM) $(NAME)
	make -C $(LIBFT_PATH) clean 

fclean:	clean
	make -C $(LIBFT_PATH) fclean

re: fclean all