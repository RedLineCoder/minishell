/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/09/15 14:57:57 by emyildir         ###   ########.fr       */
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
# define ERR_TKN_SYNTAX "-msh: syntax error near unexpected token"

typedef struct s_node t_node;

typedef enum e_tokens
{
	TKN_NONE,
	BLOCK,
	ARG,
	CMD_OP,
	REDIR,
	EXEC,
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
	OP_OR,
	OP_PIPE,
	OP_ASYNC
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

typedef struct s_node 
{
	t_cmd	*cmd;
	t_node	*left;
	t_node	*right;
} t_node;

typedef struct s_blockcmd
{
	int		type;
	char	*line;
}			t_blockcmd;

typedef struct s_execcmd
{
	int		type;
	int		out_file;
	int		in_file;
	int		fd[2];
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
	char	*s_spec;
	char	*e_spec;
}			t_redircmd;

typedef struct s_opcmd
{
	int		type;
	t_cmdop	op_type;
}			t_opcmd;

// Parser
int			parser(char *ps, t_node *node);
t_cmd		*parse_cmd(char **ps);
t_cmd		*parse_redir(char **ps,char *ts, char *te);
t_cmd		*parse_exec(char **ps, char *ts, char *te);
t_cmd		*parse_cmdop(char **ps, char *ts, char *te);
t_cmd		*parse_arg(char **ps, char *ts, char *te);
t_cmd		*parse_block(char **ps, char *ts, char *te);

// Tokenizer
t_tokens	get_token(char **ps,char **ts, char **te);
t_tokens	peek(char *ps);
bool		peek_consecutive(char *ps, char *charset, char *filter);
void		get_block(char **bs);
void		get_quote(char **qs);
void		get_operator(char **pe);

// Nodes
int			get_node_type(t_node *node);
t_cmd		*get_node_cmd(t_node *node);
t_node		*get_node(void *cmd, int nullcheck);
// Lexer
t_redir		get_redir(char *ts, char *te);
t_cmdop		get_cmdop(char *ts, char *te);
t_tokens	get_token_type(char *ts, char *te);

// Executor
int			execute_block(t_node *block, t_msh *msh);
int			execute_redir(t_execcmd *cmd, t_redircmd *redir);
int			exec_pipe(t_execcmd	*cmd);
char		**get_args_arr(t_list	*arglist);
void		executor(t_node *block, t_msh *msh);
void		exec_hdoc(t_execcmd *cmd, t_redircmd *redir);
void		close_pipe(int	fd[2]);
void		mini_panic(char *str);
void		wait_child_processes();
t_node		*get_next_block(t_node *block, int status);
// Utils
char		*get_user(void);
char		*get_cmd_path(char *command);
char		*ft_strndup(char *src, int size);
int			str_append(char **s1, char const *s2);
int			str_arr_size(char **arr);
void		free_string_array(char **arr);
void		execute_command(char *command, char **args, char **env);
void 		print_tree(t_node *node, int count);

#endif

// ls&&cat||can|meta 3<file"2" &&echo "Here'me"''"heredoc<<"and'|<>& \n\t'