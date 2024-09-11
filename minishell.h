/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/09/11 19:27:58 by emyildir         ###   ########.fr       */
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

# define SEP "|&()<> \t\n"
# define OPERATOR "|&<>"
# define SPACE " \t\n"
# define REDIRS "<>"
# define QUOTES "'\""
# define BLOCKS "()"

typedef enum e_nodes
{
	UNODE,
	BINODE
}		t_nodes;

typedef enum e_tokens
{
	TKN_NONE,
	BLOCK,
	PIPE,
	COND,
	ARG,
	REDIR,
	HDOC,
	EXEC
}			t_tokens;

typedef enum e_redir
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND
}			t_redir;

typedef enum e_cond
{
	COND_NONE,
	COND_AND,
	COND_OR,
	COND_NEG
}			t_cond;

typedef enum e_quote
{
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}			t_quote;

// Structs
typedef struct s_msh
{
	char	*user;
	char	**env;
}			t_msh;

typedef struct s_node 
{
	int		type;
} t_node;

typedef struct s_cmd
{
	int		type;
}			t_cmd;

typedef struct s_binode
{
	int		type;
	t_cmd	*cmd;
	t_node	*left;
	t_node	*right;
}			t_binode;

typedef struct s_unode
{
	int		type;
	t_cmd	*cmd;
	t_node	*next;
}			t_unode;


typedef struct s_blockcmd
{
	int		type;
	int		status;
}			t_blockcmd;

typedef struct s_execcmd
{
	int		type;
	int		out_file;
	int		in_file;
	t_list	*args;
	t_list	*redirs;
	t_list	*hdocs;
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
	char	*s_file;
	char	*e_file;
}			t_redircmd;

typedef struct s_hdoccmd
{
	int		type;
	int		fd;
	char	*s_limit;
	char	*e_limit;
}			t_hdoccmd;

typedef struct s_condcmd
{
	int		type;
	t_cond	cond_type;
}			t_condcmd;

typedef struct s_pipecmd
{
	int		type;
	int		pipe[2];
}			t_pipecmd;

// Parser
t_binode	*parser(char *prompt, int type);
t_cmd		*parse_cmd(char **ps);
t_cmd		*parse_redir(char **ps, char *ts, char *te);
t_cmd		*parse_hdoc(char **ps, char *ts, char *te);
t_cmd		*parse_exec(char **ps, char *ts, char *te);
t_cmd		*parse_cond(char **ps, char *ts, char *te);
t_cmd		*parse_arg(char **ps, char *ts, char *te);
t_cmd		*parse_pipe(char **ps, char *ts, char *te);

// Tokenizer
bool		peek(char *ps, char *charset);
bool		peek_consecutive(char *ps, char *charset);
t_tokens	get_token(char **ps, char **ts, char **te);
t_tokens	peek_next(char	*ps);
t_tokens	get_token_type(char *ts, char *te);
t_binode	*get_binode(void *left, void *right);
t_unode		*get_unode(void *next);

// Lexer
int			is_redir(char *ts, char *te);
int			is_block(char *ts, char *te);
int			is_hdoc(char *ts, char *te);
int			is_append(char *ts, char *te);
int			is_cond(char *ts, char *te);

// Executor
void		execcmd(void);
void		mini_panic(void);

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
