NAME = minishell

READLINE = lib/readline/lib/libreadline.a
RL_FLAGS = -I${PWD}/lib/readline/include/ -lreadline -L${PWD}/lib/readline/lib 
	
LIBFT_PATH = lib/libft
LIBFT = $(LIBFT_PATH)/libft.a

GNL_PATH = lib/gnl
GNL = $(GNL_PATH)/gnl.a

LIBRARIES = $(GNL) $(LIBFT) $(READLINE)

BONUS_PATH = bonus
MANDATORY_PATH = mandatory

UTILS_PATH = $(MANDATORY_PATH)/utils
UTILS_PARSER_PATH = $(UTILS_PATH)/parser
UTILS_EXECUTOR_PATH = $(UTILS_PATH)/executor
UTILS_EXPANDER_PATH = $(UTILS_PATH)/expansions
UTILS_PARSER_SOURCES = $(UTILS_PARSER_PATH)/init_list.c $(UTILS_PARSER_PATH)/tokenizer.c $(UTILS_PARSER_PATH)/lexer.c
UTILS_EXPANDER_SOURCES = $(UTILS_EXPANDER_PATH)/dollar.c $(UTILS_EXPANDER_PATH)/expander.c $(UTILS_EXPANDER_PATH)/wildcard.c $(UTILS_EXPANDER_PATH)/word_split.c $(UTILS_EXPANDER_PATH)/quotes.c
UTILS_EXECUTOR_SOURCES = $(UTILS_EXECUTOR_PATH)/processes.c $(UTILS_EXECUTOR_PATH)/executes.c $(UTILS_EXECUTOR_PATH)/commands.c $(UTILS_EXECUTOR_PATH)/redirections.c $(UTILS_EXECUTOR_PATH)/heredocs.c
UTILS_SOURCES = $(UTILS_PATH)/environment.c $(UTILS_PATH)/string_utils.c $(UTILS_PATH)/tree_utils.c $(UTILS_EXECUTOR_SOURCES) $(UTILS_EXPANDER_SOURCES) $(UTILS_PARSER_SOURCES) 
HELPERS_PATH = $(MANDATORY_PATH)/helpers
HELPERS_SOURCES = $(HELPERS_PATH)/environment.c $(HELPERS_PATH)/executor.c $(HELPERS_PATH)/parser.c $(HELPERS_PATH)/signals.c
BUILTINS_PATH = $(MANDATORY_PATH)/builtins
BUILTINS = $(BUILTINS_PATH)/cd.c $(BUILTINS_PATH)/exit.c $(BUILTINS_PATH)/pwd.c $(BUILTINS_PATH)/echo.c $(BUILTINS_PATH)/export.c $(BUILTINS_PATH)/unset.c $(BUILTINS_PATH)/env.c

BONUS_UTILS_PATH = $(BONUS_PATH)/utils
BONUS_UTILS_PARSER_PATH = $(BONUS_UTILS_PATH)/parser
BONUS_UTILS_EXECUTOR_PATH = $(BONUS_UTILS_PATH)/executor
BONUS_UTILS_EXPANDER_PATH = $(BONUS_UTILS_PATH)/expansions
BONUS_UTILS_PARSER_SOURCES = $(BONUS_UTILS_PARSER_PATH)/init_list_bonus.c $(BONUS_UTILS_PARSER_PATH)/tokenizer_bonus.c $(BONUS_UTILS_PARSER_PATH)/lexer_bonus.c
BONUS_UTILS_EXPANDER_SOURCES = $(BONUS_UTILS_EXPANDER_PATH)/dollar_bonus.c $(BONUS_UTILS_EXPANDER_PATH)/expander_bonus.c $(BONUS_UTILS_EXPANDER_PATH)/wildcard_bonus.c $(BONUS_UTILS_EXPANDER_PATH)/word_split_bonus.c $(BONUS_UTILS_EXPANDER_PATH)/quotes_bonus.c
BONUS_UTILS_EXECUTOR_SOURCES = $(BONUS_UTILS_EXECUTOR_PATH)/processes_bonus.c $(BONUS_UTILS_EXECUTOR_PATH)/executes_bonus.c $(BONUS_UTILS_EXECUTOR_PATH)/commands_bonus.c $(BONUS_UTILS_EXECUTOR_PATH)/redirections_bonus.c $(BONUS_UTILS_EXECUTOR_PATH)/heredocs_bonus.c
BONUS_UTILS_SOURCES = $(BONUS_UTILS_PATH)/environment_bonus.c $(BONUS_UTILS_PATH)/string_utils_bonus.c $(BONUS_UTILS_PATH)/tree_utils_bonus.c $(BONUS_UTILS_EXECUTOR_SOURCES) $(BONUS_UTILS_EXPANDER_SOURCES) $(BONUS_UTILS_PARSER_SOURCES) 
BONUS_HELPERS_PATH = $(BONUS_PATH)/helpers
BONUS_HELPERS_SOURCES = $(BONUS_HELPERS_PATH)/environment_bonus.c $(BONUS_HELPERS_PATH)/executor_bonus.c $(BONUS_HELPERS_PATH)/parser_bonus.c $(BONUS_HELPERS_PATH)/signals_bonus.c
BONUS_BUILTINS_PATH = $(BONUS_PATH)/builtins
BONUS_BUILTINS = $(BONUS_BUILTINS_PATH)/cd_bonus.c $(BONUS_BUILTINS_PATH)/exit_bonus.c $(BONUS_BUILTINS_PATH)/pwd_bonus.c $(BONUS_BUILTINS_PATH)/echo_bonus.c $(BONUS_BUILTINS_PATH)/export_bonus.c $(BONUS_BUILTINS_PATH)/unset_bonus.c $(BONUS_BUILTINS_PATH)/env_bonus.c

SOURCES = $(MANDATORY_PATH)/main.c $(UTILS_SOURCES) $(HELPERS_SOURCES) $(BUILTINS)
HEADERS = $(MANDATORY_PATH)/minishell.h

BONUS_SOURCES = $(BONUS_PATH)/main_bonus.c $(BONUS_UTILS_SOURCES) $(BONUS_HELPERS_SOURCES) $(BONUS_BUILTINS)
BONUS_HEADERS = $(BONUS_PATH)/minishell_bonus.h

CC = cc
CFLAGS= -Wall -Wextra -Werror -lncurses -D READLINE_LIBRARY=1 

all: $(NAME)

$(NAME): $(LIBRARIES) $(SOURCES) $(HEADERS)
		$(CC) $(SOURCES) $(LIBRARIES) ${RL_FLAGS} $(CFLAGS) -o ${NAME}

bonus: $(LIBRARIES) $(BONUS_SOURCES) $(BONUS_HEADERS)
		$(CC) $(BONUS_SOURCES) $(LIBRARIES) ${RL_FLAGS} $(CFLAGS) -o ${NAME}

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
