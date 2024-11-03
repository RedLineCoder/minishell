NAME = minishell

READLINE_PATH = lib/readline
READLINE = lib/readline/lib/libreadline.a
RL_FLAGS = -I${PWD}/lib/readline/include/ -lreadline -L${PWD}/lib/readline/lib 
	
LIBFT_PATH = lib/libft
LIBFT = $(LIBFT_PATH)/libft.a

GNL_PATH = lib/gnl
GNL = $(GNL_PATH)/gnl.a

LIBRARIES = $(GNL) $(LIBFT) $(READLINE)

UTILS_PATH = utils
UTILS_PARSER_PATH = $(UTILS_PATH)/parser
UTILS_EXECUTOR_PATH = $(UTILS_PATH)/executor
UTILS_EXPANDER_PATH = $(UTILS_PATH)/expansions
UTILS_PARSER_SOURCES = $(UTILS_PARSER_PATH)/init_list.c $(UTILS_PARSER_PATH)/tokenizer.c $(UTILS_PARSER_PATH)/lexer.c
UTILS_EXPANDER_SOURCES = $(UTILS_EXPANDER_PATH)/dollar.c $(UTILS_EXPANDER_PATH)/expander.c $(UTILS_EXPANDER_PATH)/wildcard.c $(UTILS_EXPANDER_PATH)/word_split.c $(UTILS_EXPANDER_PATH)/quotes.c
UTILS_EXECUTOR_SOURCES = $(UTILS_EXECUTOR_PATH)/processes.c $(UTILS_EXECUTOR_PATH)/executes.c $(UTILS_EXECUTOR_PATH)/commands.c $(UTILS_EXECUTOR_PATH)/redirections.c $(UTILS_EXECUTOR_PATH)/heredocs.c
UTILS_SOURCES = $(UTILS_PATH)/environment.c $(UTILS_PATH)/string_utils.c $(UTILS_PATH)/tree_utils.c $(UTILS_EXECUTOR_SOURCES) $(UTILS_EXPANDER_SOURCES) $(UTILS_PARSER_SOURCES) 
HELPERS_PATH = helpers
HELPERS_SOURCES = $(HELPERS_PATH)/environment.c $(HELPERS_PATH)/executor.c $(HELPERS_PATH)/parser.c $(HELPERS_PATH)/signals.c
BUILTINS_PATH = builtins
BUILTINS = $(BUILTINS_PATH)/cd.c $(BUILTINS_PATH)/exit.c $(BUILTINS_PATH)/pwd.c $(BUILTINS_PATH)/echo.c $(BUILTINS_PATH)/export.c $(BUILTINS_PATH)/unset.c $(BUILTINS_PATH)/env.c

SOURCES = main.c $(UTILS_SOURCES) $(HELPERS_SOURCES) $(BUILTINS)
HEADERS = minishell.h

CC = cc
CFLAGS= -Wall -Wextra -Werror -lncurses -D READLINE_LIBRARY=1 

all: $(NAME)

$(NAME): $(LIBRARIES) $(SOURCES) $(HEADERS)
		$(CC) $(SOURCES) $(LIBRARIES) ${RL_FLAGS} $(CFLAGS) -o ${NAME}

bonus: all

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
		make -C $(LIBFT_PATH) fclean
		make -C $(GNL_PATH) fclean
		$(RM) -rf $(READLINE_PATH)

fclean: clean
		$(RM) $(NAME)

re: fclean all
