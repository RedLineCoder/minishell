/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/09/28 15:41:28 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/gnl/get_next_line.h"
# include "lib/libft/libft.h"
# include <fcntl.h>
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
# define DIGITS "0123456789"
# define ERR_TKN "-msh: syntax error near unexpected token "

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
	int			status;
	char		*user;
	char		**env;
}				t_msh;

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
	char		*s_spec;
	char		*e_spec;
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
bool			peek_consecutive(char *ps, char *pe, char *charset,
					char *filter);
char			*pass_quote(char *qs, char *pe);
char			*pass_block(char *bs, char *pe);
char			*get_operator(char *te);

// Lexer
t_redir			get_redir(char *ts, char *te);
t_logicop		get_logicop(char *ts, char *te);
t_tokens		get_token_type(char *ts, char *te);

// Executor
int				dup_io(int input_fd, int output_fd, int close_fd);
int				executor(t_cmd *cmd, t_msh *msh);
void			mini_panic(char *str);
char			**get_args_arr(t_list *arglist);

// Utils
char			*get_user(void);
char			*get_cmd_path(char *command);
char			*ft_strndup(char *src, int size);
int				str_append(char **s1, char const *s2);
int				str_arr_size(char **arr);
void			free_string_array(char **arr);
void			execute_command(char *command, char **args, char **env);

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