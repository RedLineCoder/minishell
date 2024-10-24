/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/10/24 21:06:44 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>
# include <termios.h>
# include <sys/ioctl.h>
# include "lib/gnl/get_next_line.h"
# include "lib/libft/libft.h"
# include "lib/readline/include/readline/readline.h"
# include "lib/readline/include/readline/history.h"

# define SEP "|&()<> \t\n"
# define OPERATOR "|&<>"
# define SPACES " \t\n"
# define REDIRS "<>"
# define QUOTES "'\""
# define BLOCKS "()"
# define DIGITS "0123456789"
# define ERR_TKN "-msh: syntax error near unexpected token "
# define MSH_TAG "msh-1.0"

# define ERR_TAG "-msh"
# define ERR_TOO_MANY_ARG "too many arguments\n"
# define ERR_EXIT_NUM_REQUIRED "numeric argument required\n"
# define ERR_CMD_NOTFOUND "command not found\n"
# define ERR_CMD_ISDIR "Is a directory\n"
# define ERR_CD_HOME_NOT_SET "HOME not set\n"
# define EXIT_CMD_NOTFOUND 127
# define EXIT_CMD_NOTEXECUTABLE 126

typedef struct stat t_stat;
typedef struct sigaction t_action;

typedef enum e_cmdtype
{
	ROOT,
	SUBSHELL,
	LOGIC,
	PIPE,
	EXEC,
	REDIR
}				t_cmdtype;

typedef enum e_tokens
{
	TKN_NONE,
	TKN_IN,
	ARG,
	REDIR_OP,
	BLK_OP,
	BLK_CLS,
	LOGIC_OP,
	PIPE_OP,
	ERR_QUOTE
}				t_tokens;

typedef enum e_redir
{
	REDIR_NONE,
	REDIR_OUTPUT,
	REDIR_INPUT,
	REDIR_APPEND,
	REDIR_HDOC
}				t_redir;

typedef enum e_logicop
{
	OP_NONE,
	OP_AND,
	OP_OR
}				t_logicop;

typedef enum e_builtins
{
	BUILTIN_NONE,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT,
	BUILTIN_STATUS
}	t_builtins;

typedef	enum e_job
{
	NOTHING,
	WAITING_INPUT,
	WAITING_CMD,
	EXECUTING_CMD,
	EXECUTING_PIPE,
	EXECUTING_HDOC,
}	t_job;

// Structs
typedef struct s_part
{
	char		*lfts;
	char		*lfte;
	char		*rghts;
	char		*rghte;
}				t_part;

typedef struct s_pattern
{
	int		diff;
	int		e_size;
	int		s_size;
	char	*arg;
	char	*file;
}				t_pattern;

typedef	struct s_write
{
	int		a_i;
	int		e_i;
	char	qs;
	char	qd;
}				t_write;


typedef struct s_env{
	char	*key;
	char	*pair;
}				t_env;

typedef struct s_cmd
{
	int			type;
}				t_cmd;

typedef struct s_blockcmd
{
	int			type;
	int			out_file;
	int			in_file;
	t_cmd		*subshell;
	t_list		*redirs;
}				t_blockcmd;

typedef struct s_execcmd
{
	int			type;
	int			out_file;
	int			in_file;
	t_list		*args;
	t_list		*redirs;
}				t_execcmd;

typedef struct s_redircmd
{
	int			type;
	int			redir_type;
	int			fd;
  int     old_fd;
	int			pipe[2];
	t_list		*args;
}				t_redircmd;

typedef struct s_pipecmd
{
	int			type;
	int			fd[2];
	t_list		*pipelist;
}				t_pipecmd;

typedef struct s_logiccmd
{
	int			type;
	t_logicop	op_type;
	t_cmd		*left;
	t_cmd		*right;
}				t_logiccmd;

typedef struct s_msh
{
	int		last_status;
	int		exit_flag;
	t_job	current_job;
	char	*user;
	t_cmd	*tree_root;
	t_list	*env;
}			t_msh;

// Parser
int			parser(char *ps, char *pe, t_cmd **cmd);
int			parse_redirs(char *ps, char *pe, int block, t_list **redirs);
int			parse_args(char *ps, char *pe, t_list **args);
int			init_pipes(char *ps, char *pe, t_list **pipelist);
int			syntax_panic(char *ps);
void		clean_tree(void *cmd);
t_part		ft_divide(char *s, char *e, t_tokens tkn, int rev);

// Tokenizer
int			pass_block(char *bs, char **be, char *pe);
int			pass_quote(char **qs, char *pe, char *quotes);
t_tokens	get_token(char **ps, char **pe, char **ts, char **te);
t_tokens	peek(char *ps, char *pe, t_tokens token);

// Lexer
t_redir		get_redir(char *ts, char *te);
t_logicop	get_logicop(char *ts, char *te);
t_tokens	get_token_type(char *ts, char *te);

// Expander
int		is_expanded(t_list *explst, int index);
int		set_exptrack(t_list **explst, int start, int end);
int		expand_wildcard(t_list **expanded, t_list *explst, char *arg);
char	*expand_dollar(char *arg, t_list **explst, t_msh *msh);
char	*unquote_arg(t_list *explst, char *arg);
t_list	*expander(t_list *args, t_msh *msh);

// Executor
int		execute_redir(t_redircmd *redir, t_msh *msh);
int		execute_logic(t_logiccmd *opcmd, t_msh *msh);
int		execute_exec(t_execcmd *exec, t_msh *msh, int builtin);
int		execute_block(t_blockcmd *block, t_msh *msh);
int		execute_pipe(t_list *pipelist, t_msh *msh);
int		execute_builtin(int builtin, char **args, t_msh *msh);
int		run_command(char *command, char **args, t_list	*env);
int		run_heredoc(t_redircmd *redir, t_msh *msh);
int		handle_redirects(t_list *redirs, t_msh *msh);
int     handle_back_redirects(t_list *redirs);
int		mini_panic(char *title, char *content, int exit_flag);
int		get_builtin(t_execcmd *exec);
int		get_redir_flags(t_redir type);
int		handle_heredocs(t_cmd *root, t_msh *msh);
char	*get_executable_path(char *command, t_list *env);
char	**get_args_arr(t_list	*arglist);
char	**get_env_arr(t_list *mshenv);
void	executor(t_cmd *block, t_msh *msh);
void	close_pipe(int	fd[2]);
void	print_env(t_list *lst, int quotes, int hide_null);
pid_t	execute_cmd(t_cmd *cmd, t_msh *msh, int *status, int pipe[2]);

int		tree_map(t_cmd *cmd, void *payload, int (*f)(t_cmd *, void *));
char	*get_prompt(t_msh *msh);
void	free_list(t_list *lst);

//Signals
void    handle_signals();

//Process Utils
pid_t	create_child(int pipe[2], int fd);
int		wait_child_processes(int pid);

//String Utils
int		str_arr_size(char **arr);
int		str_append(char **s1, char const *s2);
int		lst_addback_content(t_list **lst, void *content);
char	*ft_strndup(char *src, int size);
char	*str_include(const char *s, int c);
void	free_string_array(char **arr);

//Builtins
int		builtin_cd(int args_size, char **args, t_msh *msh);
int		builtin_exit(int args_size, char **args, t_msh *msh);
int		builtin_pwd(int args_size, char **args, t_msh *msh);
int		builtin_echo(int args_size, char **args, t_msh *msh);
int		builtin_export(int args_size, char **args, t_msh *msh);
int		builtin_unset(int args_size, char **args, t_msh *msh);
int		builtin_env(int args_size, char **args, t_msh *msh);
int		builtin_status(int args_size, char **args, t_msh *msh);

//Environment
int		unset_env(t_list **root, char *key);
int		set_env(t_list **root, char *key, char *pair);
void	destroy_env(t_list *lst);
void	destroy_environment(t_list	*mshenv);
void	init_environment(t_list **msh, char **env);
char	*get_env(t_list *root, char *key);
t_list	*get_env_node(t_list *lst, char *key);

extern t_job	job;

#endif