/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/10/18 17:56:32 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void treeprint(t_cmd *root, int level)
{
        if (root == NULL)
			return;
        for (int i = 0; i < level; i++)
		{
			
                printf(i == level - 1 ? "|-" : "  ");
		}
		if (root->type == LOGIC)
		{
			t_logicop type = ((t_logiccmd *)root)->op_type;
			switch (type)
			{
				case 1:	
					printf("AND\n");
					break;
				case 2:
					printf("OR\n");
					break;
				default:
					printf("NONE\n");
					break;
			}
			treeprint(((t_logiccmd *)root)->left, level + 1);
			treeprint(((t_logiccmd *)root)->right, level + 1);
		}
		if (root->type == SUBSHELL)
		{
			printf("SUBSHELL - ");
			printf("REDIRS: ");
			if (!((t_blockcmd *)root)->redirs)
				printf("NONE");
			while (((t_blockcmd *)root)->redirs)
			{
				while (((t_redircmd *)root)->args)
				{
					printf("%s ", (char *)((t_redircmd *)root)->args->content);
					((t_redircmd *)root)->args = ((t_redircmd *)root)->args->next;
				}
				t_redir rtype = ((t_redircmd *)((t_blockcmd *)root)->redirs->content)->redir_type;
				switch (rtype)
				{
					case REDIR_INPUT:	
						printf("INPUT - ");
						break;
					case REDIR_OUTPUT:
						printf("OUTPUT - ");
						break;
					case REDIR_APPEND:
						printf("APPEND - ");
						break;
					case REDIR_HDOC:
						printf("HEREDOC - ");
						break;
					default:
						printf("NONE - ");
						break;
				}
				printf("FD:%i", ((t_redircmd *)((t_blockcmd *)root)->redirs->content)->fd);
				((t_blockcmd *)root)->redirs = ((t_blockcmd *)root)->redirs->next;
			}
			printf("\n");
			treeprint(((t_blockcmd *)root)->subshell, level + 1);
		}
        if (root->type == PIPE)
		{
			printf("PIPELIST\n");
			while (((t_pipecmd *)root)->pipelist)
			{
				treeprint((((t_pipecmd *)root)->pipelist)->content, level + 1);
				((t_pipecmd *)root)->pipelist = ((t_pipecmd *)root)->pipelist->next;
			}
		}
		if (root->type == EXEC)
		{
			printf("EXEC = ");
			printf("CMD: ");
			if (!((t_execcmd *)root)->args)
				printf("NONE, ");
			else
			{
				printf("%s ", (char *)(((t_execcmd *)root)->args->content));
				((t_execcmd *)root)->args = ((t_execcmd *)root)->args->next;
			}
			printf("ARGS: ");
			if (!((t_execcmd *)root)->args)
				printf("NONE, ");
			while (((t_execcmd *)root)->args)
			{
				printf("%s ", (char *)((t_execcmd *)root)->args->content);
				((t_execcmd *)root)->args = ((t_execcmd *)root)->args->next;
			}
			printf("REDIRS: ");
			if (!((t_execcmd *)root)->redirs)
				printf("NONE");
			while (((t_execcmd *)root)->redirs)
			{
				while (((t_execcmd *)root)->redirs)
				{
					printf("%s ", (char *)((t_execcmd *)root)->redirs->content);
					((t_execcmd *)root)->redirs = ((t_execcmd *)root)->redirs->next;
				}
				t_redir rtype = ((t_redircmd *)((t_execcmd *)root)->redirs->content)->redir_type;
				switch (rtype)
				{
					case REDIR_INPUT:	
						printf("INPUT ");
						break;
					case REDIR_OUTPUT:
						printf("OUTPUT ");
						break;
					case REDIR_APPEND:
						printf("APPEND ");
						break;
					case REDIR_HDOC:
						printf("HEREDOC ");
						break;
					default:
						printf("NONE ");
						break;
				}
				printf("FD:%i ", ((t_redircmd *)((t_execcmd *)root)->redirs->content)->fd);
				((t_execcmd *)root)->redirs = ((t_execcmd *)root)->redirs->next;
			}
			printf("\n");
		}
} 

int	mini_panic(char *title, char *content, int status)
{
	char	*tag;

	tag = ft_strdup(ERR_TAG);
	if (!tag)
		perror(ERR_TAG);
	if (title)
	{
		str_append(&tag, ": ");
		str_append(&tag, title);
	}
	if (content)
	{
		ft_putstr_fd(tag, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(content, STDERR_FILENO);
	}
	else
		perror(tag);
	free(tag);
	return (status);
}
 
int	main(int argc, char **argv, char **env)
{
	t_msh *const	msh = &(t_msh){0};
	char			*prompt;
	char			*line;
	int				status;

	(void)argv, (void)argc;
	init_environment(&msh->env, env);
	while (!msh->exit_flag)
	{
		prompt = get_prompt(msh);
		if (!prompt)
		{
			mini_panic(ERR_TAG, NULL, -1);
			continue;
		}
		if (isatty(fileno(stdin)))
			line = readline(prompt);
		else
		{
			line = get_next_line(fileno(stdin));
			line = ft_strtrim(line, "\n");
		} 
		if (!line)
			exit(msh->last_status);
		if (ft_strlen(line) == 0)
			continue;
		add_history(line);
		t_cmd *root;
		status = parser(line, line + ft_strlen(line), &root);
		if (!status)
			executor(root, msh);
		else 
			msh->last_status = status;
		//treeprint(root, 0);
		free(line);
		if (msh->user)
		{
			free(msh->user);
			free(prompt);
		}
	}
	rl_clear_history();
	destroy_environment(msh->env);
	//printf("exit\n");
	return (msh->last_status);
}
