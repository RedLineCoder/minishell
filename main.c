/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/10/13 19:44:16 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void treeprint(t_cmd *root, int level)
{
        if (root == NULL)
			return;
        for (int i = 0; i < level; i++)
                printf(i == level - 1 ? "|-" : "  ");
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
} */

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

char	*get_prompt(t_msh *msh)
{
	bool	is_tilde;
	int		size;
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
	if (!str_append(&prompt, " $ "))
		return (free(prompt), NULL);
	return (prompt);
}
 
int	main(int argc, char **argv, char **env)
{
	t_msh *const	msh = &(t_msh){0};
	char			*prompt;
	char			*line;

	(void)argv, (void)argc;
	init_environment(&msh->env, env);
	while (!msh->exit_flag)
	{
		prompt = MSH_TAG;
		msh->user = get_user(msh->env);
		if (msh->user)
			prompt = get_prompt(msh);
		if (!prompt)
		{
			free(msh->user);
			mini_panic(NULL, "An error occured.", -1);
		}
		line = readline(prompt);
		if (!line) 
			exit(0);
		if (ft_strlen(line) == 0)
			continue;
		add_history(line);
		t_cmd *root;
		msh->last_status = parser(line, line + ft_strlen(line), &root);
		if (!msh->last_status)
			executor(root, msh);
		//printf("%p\n", root->right);
		//treeprint(root, 0);
		/* t_list	*test = ft_calloc(sizeof(t_list), 1);
		test->content = line;
		test = expander(test, msh);
		printf("%s\n", test->content); */
		free(line);
		if (msh->user)
		{
			free(msh->user);
			free(prompt);
		}
		//clean_tree(root);
	}
	rl_clear_history();
	destroy_environment(msh->env);
	printf("exit\n");
	return (msh->last_status);
}

/* void __attribute__ ((destructor)) sa()
{
	system("leaks minishell");
} */