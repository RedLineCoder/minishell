/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/08/29 20:17:23 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/libft/libft.h"
# include "lib/gnl/get_next_line.h"
# include <limits.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>

// Enumerators

typedef enum e_cmds
{
	CMD_NONE,
	PIPE,
	COND,
	ARG,
	BLOCK,
	REDIR,
	HDOC,
	DOLLAR
}			t_cmds;

typedef enum e_redir
{
	REDIR_INPUT,
	REDIR_OUTPUT
}			t_redir;

typedef enum e_cond
{
	COND_NONE,
	COND_AND,
	COND_OR,
	COND_NEG
}			t_cond;

# define ARGSEP " \t\n"

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
	int		type;
	char	*ps;
	char	*pe;
	t_cmd	*left;
	t_cmd	*right;
}	t_binode;

typedef	struct s_unode
{
	int		type;
	char	*ps;
	char	*pe;
	t_cmd	*bottom;
}	t_unode;

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
	char	*s_file;
	char	*e_file;
}			t_redircmd;

typedef struct s_hdoccmd
{
	int		type;
	char	*limit;
	char	*s_limit;
	char	*e_limit;
}			t_hdoccmd;

typedef struct s_dollarcmd
{
	int		type;
	char	*s_key;
	char	*e_key;
}			t_dollarcmd;

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

typedef struct s_blockcmd
{
	int		type;
}			t_blockcmd;

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

// Executor
void		execcmd(void);
void		mini_panic(void);

// Utils
char	*get_user();
char	*get_cmd_path(char *command);
int		str_append(char **s1, char const *s2);
int		str_arr_size(char **arr);
void	parser(char *prompt);
void	free_string_array(char **arr);
void	execute_command(char *command, char **args, char **env);

#endif
