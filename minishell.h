/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/08/27 17:47:45 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>

// Enumerators

typedef enum e_cmds
{
	PIPE,
	COND,
	EXEC,
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

typedef struct s_exec_cmd
{
	int		type;
	char	**args;
	char	*out_file;
	char	*in_file;
}			t_exec_cmd;

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

// Executor
void		execcmd(void);
void		tokenize(void);
void		mini_panic(void);

#endif
