/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/09/24 17:57:20 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/gnl/get_next_line.h"
# include "lib/libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>

# define SEP "|&()<> \t\n"
# define OPERATOR "|&<>"
# define SPACE " \t\n"
# define REDIRS "<>"
# define QUOTES "'\""
# define BLOCKS "()"
# define DIGITS "0123456789"
# define ERR_TKN "-msh: syntax error near unexpected token "
# define ERR_MISS "-msh: syntax error missing token "

typedef enum e_cmdtype
{
	ROOT,
	SUBSHELL,
	LOGIC,
	PIPE,
	EXEC,
	REDIR
}			t_cmdtype;

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
	ERR_TOKEN = 258
}			t_tokens;

typedef enum e_redir
{
	REDIR_NONE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HDOC
}			t_redir;

typedef enum e_cmdop
{
	OP_NONE,
	OP_AND,
	OP_OR
}			t_cmdop;

typedef enum e_builtins
{
	BUILTIN_NONE,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BULTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
}		t_builtins;

// Structs
typedef	struct s_lnsplit
{
	char	*lfts;
	char	*lfte;
	char	*rghts;
	char	*rghte;
}			t_lnsplit;

typedef struct s_msh
{
	int		last_status;
	char	*user;
	char	**env;
}			t_msh;

typedef struct s_cmd
{
	int		type;
}			t_cmd;

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
	int		type;
	int		**pipes;
	t_list	*pipelist;
}			t_pipecmd;

typedef struct s_opcmd
{
	int		type;
	t_cmdop	op_type;
	t_cmd	*left;
	t_cmd	*right;
}			t_opcmd;

// Parser
t_cmd		*parser(char *ps, char *pe);
t_cmd		*parse_cmd(char **ps, char **pe);
t_cmd		*parse_redir(char **ps, char **pe, char *ts, char *te);
t_cmd		*parse_exec(char **ps, char **pe, char *ts, char *te);
t_cmd		*parse_cmdop(char **ps, char **pe, char *ts, char *te);
t_cmd		*parse_arg(char **ps, char **pe, char *ts, char *te);
t_lnsplit	ft_lnsplit(char *line, char *end, t_tokens token, int reverse);
char		*pass_block(char *bs, char *pe);

// Tokenizer
t_tokens	get_token(char **ps, char **pe, char **ts, char **te);
t_tokens	peek(char *ps, char *pe, t_tokens token);
bool		peek_consecutive(char *ps, char *pe, char *charset, char *filter);
char		*pass_quote(char *qs, char *pe);
char		*pass_block(char *bs, char *pe);
char		*get_operator(char *te);

// Lexer
t_redir		get_redir(char *ts, char *te);
t_cmdop		get_logicop(char *ts, char *te);
t_tokens	get_token_type(char *ts, char *te);

// Executor
int			init_pipes(t_pipecmd *pipecmd);
pid_t		execute_cmd(t_cmd *cmd, t_msh *msh);
void		mini_panic(char *str);
char		**get_args_arr(t_list	*arglist);
void		executor(t_cmd *block, t_msh *msh);
void		close_pipe(int	fd[2]);
void		mini_panic(char *str);
void		wait_child_processes();
int			execute_redir(t_execcmd *cmd, t_redircmd *redir);
void 		exec_hdoc(t_redircmd *redir);

// Utils
char		*get_user(void);
char		*get_cmd_path(char *command);
char		*ft_strndup(char *src, int size);
int			str_append(char **s1, char const *s2);
int			str_arr_size(char **arr);
void		free_string_array(char **arr);
void		execute_command(char *command, char **args, char **env);

#endif

// ls&&cat||can|meta 3<file"2" &&echo "Here'me"''"heredoc<<"and'|<>& \n\t'
// (ls | ls && cat < redir) || ls && cmd || (echo "afbf|&&" || ls)

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