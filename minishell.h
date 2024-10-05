/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/10/05 15:30:40 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/gnl/get_next_line.h"
# include "lib/libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/stat.h>
# include <readline/history.h>
# include <readline/readline.h>

# define SEP "|&()<> \t\n"
# define OPERATOR "|&<>"
# define SPACE " \t\n"
# define REDIRS "<>"
# define QUOTES "'\""
# define BLOCKS "()"
# define DIGITS "0123456789"
# define ERR_TKN "-msh: syntax error near unexpected token "
# define ERR_TAG "-msh"
# define MSH_TAG "msh $ "

typedef struct stat t_stat;

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
	BUILTIN_EXIT
}		t_builtins;

// Structs
typedef struct s_part
{
	char		*lfts;
	char		*lfte;
	char		*rghts;
	char		*rghte;
}				t_part;

typedef struct s_msh
{
	int		last_status;
	char	*user;
	t_list	*env;
}			t_msh;

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
	int		type;
	int		out_file;
	int		in_file;
	t_cmd	*subshell;
	t_list	*redirs;
}			t_blockcmd;

typedef struct s_execcmd
{
	int		type;
	int		out_file;
	int		in_file;
	t_list	*args;
	t_list	*redirs;
}			t_execcmd;

typedef struct s_argcmd
{
	int		type;
	char	*s_arg;
	char	*e_arg;
}			t_argcmd;

typedef struct s_redircmd
{
	int		type;
	int		redir_type;
	int		fd;
	int		pipe[2];
	char	*s_spec;
	char	*e_spec;
}			t_redircmd;

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

// Parser
int				parser(char *ps, char *pe, t_cmd **cmd);
int				parse_redirs(char *ps, char *pe, int block, t_list **redirs);
int				parse_args(char *ps, char *pe, t_list **args);
int				init_pipes(char *ps, char *pe, t_list **pipelist);
int				syntax_panic(char *ps);
void			clean_tree(void *cmd);
t_part			ft_divide(char *s, char *e, t_tokens tkn, int rev);

// Tokenizer
t_tokens		get_token(char **ps, char **pe, char **ts, char **te);
t_tokens		peek(char *ps, char *pe, t_tokens token);
int				pass_quote(char **qs, char *pe, char *quotes);
int				pass_block(char *bs, char **be, char *pe);

// Lexer
t_redir			get_redir(char *ts, char *te);
t_logicop		get_logicop(char *ts, char *te);
t_tokens		get_token_type(char *ts, char *te);

// Executor
int			wait_child_processes(int pid);
int			execute_redir(t_redircmd *redir);
int			execute_logic(t_logiccmd *opcmd, t_msh *msh);
int			execute_cmd(t_cmd *cmd, t_msh *msh, int should_fork);
int			execute_exec(t_execcmd *exec, t_msh *msh, int builtin);
void		execute_block(t_blockcmd *block, t_msh *msh);
void		execute_pipe(t_pipecmd *pipecmd, t_msh *msh);
int			execute_builtin(int builtin, char **args, t_msh *msh);
void		executor(t_cmd *block, t_msh *msh);
void		close_pipe(int	fd[2]);
void		mini_panic(char *title, char *content, int exit_flag, int exit_status);
void		print_env(t_list *lst, int quotes, int hide_null);
char		**get_args_arr(t_list	*arglist);
char		**get_env_arr(t_list *mshenv);

// Utils
char			*get_user(t_list *env);
char			*get_cmd_path(char *command, t_list *env);
char			*ft_strndup(char *src, int size);
int				str_append(char **s1, char const *s2);
int				str_arr_size(char **arr);
void			free_string_array(char **arr);
void			execute_command(char *command, char **args, t_list	*env, int silence);

//Builtins
int		builtin_cd(int args_size, char **args, t_msh *msh);
int		builtin_exit(int args_size, char **args, t_msh *msh);
int		builtin_pwd(int args_size, char **args, t_msh *msh);
int		builtin_echo(int args_size, char **args, t_msh *msh);
int		builtin_export(int args_size, char **args, t_msh *msh);
int		builtin_unset(int args_size, char **args, t_msh *msh);
int		builtin_env(int args_size, char **args, t_msh *msh);

//Environment
int		unset_env(t_list **root, char *key);
int		set_env(t_list **root, char *key, char *pair);
void	destroy_env(t_list *lst);
void	init_environment(t_list **msh, char **env);
char	*get_env(t_list *root, char *key);
t_list	*get_env_node(t_list *lst, char *key);

#endif

// ls&&cat||can|meta 3<file"2" &&echo "Here'me"''"heredoc<<"and'|<>& \n\t'
// (ls | ls && cat < redir) || ls && cmd || (echo "afbf|&&" || ls)

//EXEC
// ls|cat&&ls|echo me 3>file"2" &&echo "Here'me"''"heredoc<<"and'|<>& \n\t'


/* 

<REDIRECTION> ::=  '>' <WORD>
				|  '<' <WORD>
				|  <NUMBER> '>' <WORD>
				|  <NUMBER> '<' <WORD>
				|  '>>' <WORD>
				|  <NUMBER> '>>' <WORD>
				|  '<<' <WORD>
				|  <NUMBER> '<<' <WORD>

<PIPELINE> ::=
			<PIPELINE> '|' <NEWLINE-LIST> <PIPELINE>
		|  <COMMAND>

<CONDITION> ::=
			<CONDITION> '&&' <NEWLINE-LIST> <CONDITION>
		|  <CONDITION> '||' <NEWLINE-LIST> <CONDITION>
		|  <COMMAND>

STRUCT: <EXEC> <COND or PIPE> <EXEC>

<BS> <STRUCT> <BE>

is_block ?
is_struct ?

*/

/* void	syntax_panic(char *ps, char *msg)
{
	t_tokens	token;
	char		*ts;
	char		*te;
	char		*pe;

	pe = ps + ft_strlen(ps);
	token = get_token(&ps, &pe, &ts, &te);
	if (msg)
	{
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("'", 2);
		if (!token)
			write(2, "newline", 7);
		else
			write(2, ts, te - ts);
		ft_putendl_fd("'", 2);
	}
} */

/* int	get_operator(char **te)
{
	char	*str;

	str = *te;
	if (ft_isdigit(*str))
	{
		while (ft_isdigit(*str))
			str++;
		if (!ft_strchr(REDIRS, *str))
			return (0);
	}
	else if (!ft_strchr(OPERATOR, *str))
		return (0);
	if (*str == *(str + 1))
		str++;
	*te = str + 1;
	return (1);
} */