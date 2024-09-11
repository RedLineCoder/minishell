/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/09/11 19:42:47 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_node(t_cmd *cmd, int left, int row)
{
	if (!cmd)
		return ;
	t_tokens	token = cmd->type;
	printf("------------------------------------\n\n");
	printf("%d - ", row);
	if (left)
		printf("Left\n");
	else
		printf("Right\n");
	printf("------------------------------------\n\n");
	if (token == EXEC)
	{
		t_execcmd	*exec = (t_execcmd *)cmd;
		printf("Type : Exec\n");
		printf("Args : ");
		while (exec->args)
		{
			t_argcmd *arg = exec->args->content;
			for (int i = 0; arg->s_arg + i  < arg->e_arg; i++)
				printf("%c", arg->s_arg[i]);
			printf(" - ");
			exec->args = exec->args->next;
		}
		printf("\n\nRedirects: \n\n");
		while (exec->redirs)
		{
			t_redircmd *redir = exec->redirs->content;
			printf("File: ");
			for (int i = 0; redir->s_file + i  < redir->e_file; i++)
				printf("%c", redir->s_file[i]);
			printf("\nManipulated fd : %d\n", redir->fd);
			printf("Redir Type : ");
			if (redir->redir_type == REDIR_OUTPUT)
				printf("Output\n");
			else if(redir->redir_type == REDIR_INPUT)
				printf("Input\n");
			else if(redir->redir_type == REDIR_APPEND)
				printf("Append\n"); 
			printf("\n");
			exec->redirs = exec->redirs->next;
		}
		printf("\nHeredocs: \n\n");
		while (exec->hdocs)
		{
			t_hdoccmd *hdoc = exec->hdocs->content;
			printf("Target Fd : %d\n", hdoc->fd);
			printf("EOF : ");
			for (int i = 0; hdoc->s_limit + i  < hdoc->e_limit; i++)
				printf("%c", hdoc->s_limit[i]);
			printf("\n");
			exec->hdocs = exec->hdocs->next;
		}
	}
	else if(token == COND)
	{
		t_condcmd *cond = (t_condcmd *) cmd;
		
		printf("Type : Condition\n");
		printf("Condition Type : ");
		if (cond->cond_type == COND_OR)
			printf(" OR\n");
		else if (cond->cond_type == COND_AND)
			printf(" AND\n");
	}
	else if(token == PIPE)
		printf("Type : Pipe\n");
	printf("\n------------------------------------\n");
}

void 	print_tree(t_node *node, int count)
{
	if (node->type == BINODE)
	{
		t_binode	*binode = (t_binode *)node;
		t_unode		*left	= (t_unode *) binode->left;
		t_binode	*right = (t_binode *) binode->right;
		if (left)
			print_node(left->cmd, 1, count + 1);
		if (right)
		{
			print_node(right->cmd, 0, count + 1);
			print_tree((t_node *) right, count + 1);
		}
	}
	else if (node->type == UNODE)
	{
		t_unode	*unode = (t_unode *) node;
		if (unode)
			print_node(unode->cmd, 1, count + 1);
	}
}

void	mini_panic(void)
{
	exit(1);
}

char	*get_prompt(t_msh *msh)
{
	int		size;
	bool	is_tilde;
	char	path[PATH_MAX];
	char	*prompt;
	char	**path_splitted;

	getcwd(path, PATH_MAX);
	prompt = ft_strjoin(msh->user, " ");
	path_splitted = ft_split(path, '/');
	if (!prompt || !path_splitted)
		return (free(prompt), free_string_array(path_splitted), NULL);
	size = str_arr_size(path_splitted);
	is_tilde = (size == 2) && !ft_strncmp(path_splitted[0], "Users", 6)
		&& !ft_strncmp(path_splitted[1], msh->user, ft_strlen(msh->user));
	if (size <= 1 && (!str_append(&prompt, "/")))
		return (free_string_array(path_splitted), free(prompt), NULL);
	else if (is_tilde && !str_append(&prompt, "~"))
			return (free_string_array(path_splitted), free(prompt), NULL);
	else if (size >= 1 && !is_tilde && !str_append(&prompt, path_splitted[size - 1]))
		return (free_string_array(path_splitted), free(prompt), NULL);
	free_string_array(path_splitted);
	if (!str_append(&prompt, " % "))
		return (free(prompt), NULL);
	return (prompt);
}

int	main(int argc, char **argv, char **env)
{
	t_msh *const	msh = &(t_msh){0};
	char			*prompt;
	char			*line;

	(void)argv;
	(void)argc;
	msh->env = env;
	while (1)
	{
		msh->user = get_user();
		prompt = get_prompt(msh);
		if (!prompt)
			mini_panic();
		line = readline(prompt);
		add_history(line);
		t_node *root = (t_node *) parser(line, TKN_NONE);
		print_tree(root, 0);
		free(line);
		free(prompt);
	}
	return (0);
}
