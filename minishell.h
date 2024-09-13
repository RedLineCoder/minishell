/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/09/13 18:38:35 by emyildir         ###   ########.fr       */
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

typedef enum e_nodes
{
	UNODE,
	BINODE
}		t_nodes;

typedef enum e_tokens
{
	TKN_NONE,
	BLOCK,
	CMD_OP,
	ARG,
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
	OP_PIPE
}			t_cmdop;		

// Structs
typedef struct s_msh
{
	char	*user;
	char	**env;
}			t_msh;


typedef struct s_cmd
{
	int		type;
}			t_cmd;

typedef struct s_node 
{
	int		type;
	t_cmd	*cmd;
} t_node;

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

typedef struct s_execcmd
{
	int		type;
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
t_binode	*parser(char *ps, char *pe, int is_block);
t_cmd		*parse_cmd(char **ps, char**pe);
t_cmd		*parse_redir(char **ps, char **pe,char *ts, char *te);
t_cmd		*parse_exec(char **ps, char **pe, char *ts, char *te);
t_cmd		*parse_cmdop(char **ps, char **pe, char *ts, char *te);
t_cmd		*parse_arg(char **ps, char **pe, char *ts, char *te);
t_binode	*parse_block(char **ps, char**pe, char *ts, char *te);

// Tokenizer
t_tokens	peek(char *ps, char *pe);
bool		peek_consecutive(char *ps, char *charset, char *filter);
t_tokens	get_token(char **ps, char **pe,char **ts, char **te);
t_tokens	get_token_type(char *ts, char *te);
t_binode	*get_binode(void *left, void *right);
t_unode		*get_unode(void *cmd);

// Lexer
t_redir		get_redir(char *ts, char *te);
t_cmdop		get_cmdop(char *ts, char *te);
t_tokens	is_block(char *ts, char *te);

// Executor
int			executor(t_binode *root, t_msh *msh);
int			dup_io(int input_fd, int output_fd, int close_fd);
void		mini_panic(char *str);
char		**get_args_arr(t_list	*arglist);

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