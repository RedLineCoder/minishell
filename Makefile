NAME = minishell

READLINE = lib/readline/lib/libreadline.a
RL_FLAGS = -I${PWD}/lib/readline/include/ -lreadline -L${PWD}/lib/readline/lib 
	
LIBFT = lib/libft/libft.a
LIBFT_PATH = lib/libft

GNL = lib/gnl/gnl.a
GNL_PATH = lib/gnl

LIBRARIES = $(GNL) $(LIBFT) $(READLINE)

UTILS_PATH = utils
UTILS_PARSER_PATH = $(UTILS_PATH)/parser
UTILS_EXECUTOR_PATH = $(UTILS_PATH)/executor
UTILS_EXPANDER_PATH = $(UTILS_PATH)/expansions
UTILS_PARSER_SOURCES = $(UTILS_PARSER_PATH)/init_list.c $(UTILS_PARSER_PATH)/tokenizer.c $(UTILS_PARSER_PATH)/lexer.c
UTILS_EXPANDER_SOURCES = $(UTILS_EXPANDER_PATH)/dollar.c $(UTILS_EXPANDER_PATH)/expander.c $(UTILS_EXPANDER_PATH)/wildcard.c $(UTILS_EXPANDER_PATH)/word_split.c $(UTILS_EXPANDER_PATH)/quotes.c
UTILS_EXECUTOR_SOURCES = $(UTILS_EXECUTOR_PATH)/processes.c $(UTILS_EXECUTOR_PATH)/executes.c $(UTILS_EXECUTOR_PATH)/commands.c $(UTILS_EXECUTOR_PATH)/redirections.c $(UTILS_EXECUTOR_PATH)/heredocs.c
UTILS_SOURCES = $(UTILS_PATH)/environment.c $(UTILS_PATH)/mem_utils.c $(UTILS_PATH)/prompt.c $(UTILS_PATH)/string_utils.c $(UTILS_PATH)/tree.c $(UTILS_EXECUTOR_SOURCES) $(UTILS_EXPANDER_SOURCES) $(UTILS_PARSER_SOURCES) 
HELPERS_PATH = helpers
HELPERS_SOURCES = $(HELPERS_PATH)/environment.c $(HELPERS_PATH)/executor.c $(HELPERS_PATH)/parser.c $(HELPERS_PATH)/signals.c
BUILTINS = builtins/cd.c builtins/exit.c builtins/pwd.c builtins/echo.c builtins/export.c builtins/unset.c builtins/env.c
SOURCES = main.c $(UTILS_SOURCES) $(HELPERS_SOURCES) $(BUILTINS)
HEADERS = minishell.h

CC = cc
CFLAGS= -Wall -Wextra -Werror -lncurses -g -D READLINE_LIBRARY=1 

all: $(NAME)

$(NAME): $(LIBRARIES) $(SOURCES) $(HEADERS) Makefile
		$(CC) $(SOURCES) $(LIBRARIES) ${RL_FLAGS} $(CFLAGS) -o ${NAME}

$(READLINE):
		curl -O https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
		tar -xvf readline-8.2.tar.gz
		cd readline-8.2 && ./configure --prefix=${PWD}/lib/readline && make && make install 
		$(RM) -r readline-8.2.tar.gz readline-8.2

$(LIBFT): 
		make -C $(LIBFT_PATH) all bonus

$(GNL):
		make -C $(GNL_PATH)

clean:
		make -C $(LIBFT_PATH) clean 
		make -C $(GNL_PATH) clean

fclean: clean
		$(RM) $(NAME)
		make -C $(LIBFT_PATH) fclean
		make -C $(GNL_PATH) fclean

re: fclean all
