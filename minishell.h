/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/09/11 00:36:13 by emyildir         ###   ########.fr       */
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

typedef enum e_cmds
{
	CMD_NONE,
	BLOCK,
	PIPE,
	COND,
	ARG,
	REDIR,
	HDOC
}			t_cmds;

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

typedef struct s_binode
{
	t_cmd	*cmd;
	t_cmd	*left;
	t_cmd	*right;
}			t_binode;

typedef struct s_unode
{
	t_cmd	*cmd;
	t_cmd	*next;
}			t_unode;

typedef struct s_blockcmd
{
	int		type;
	int		status;
}			t_blockcmd;

typedef struct s_execcmd
{
	int		type;
	char	**args;
	char	*out_file;
	char	*in_file;
}			t_execcmd;

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
	char	*limit;
	char	*s_limit;
	char	*e_limit;
}			t_hdoccmd;

typedef struct s_condcmd
{
	int		type;
	t_cond	cond;
}			t_condcmd;

typedef struct s_pipecmd
{
	int		type;
	int		pipe[2];
}			t_pipecmd;

// Parser
void		parsecmd(void);
void		parsepipe(void);
void		parseredir(void);
void		parseblock(void);
void		parsehdoc(void);
void		parsedollar(void);

// Parser Utils
bool		peek(char *ps, char *charset);
t_cmds		get_token(char **ps, char **ts, char **te);

// Lexer
int			is_redir(char *ts, char *te);

// Executor
void		execcmd(void);
void		mini_panic(void);

// Utils
char		*get_user(void);
char		*get_cmd_path(char *command);
int			str_append(char **s1, char const *s2);
int			str_arr_size(char **arr);
void		parser(char *prompt);
void		free_string_array(char **arr);
void		execute_command(char *command, char **args, char **env);

#endif
