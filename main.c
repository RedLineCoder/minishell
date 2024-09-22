/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/09/22 14:09:06 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void treeprint(t_cmd *root, int level)
{
        if (root == NULL)
			return;
        for (int i = 0; i < level; i++)
                printf(i == level - 1 ? "|-" : "  ");
		if (root->type == LOGIC)
		{
			t_cmdop type = ((t_opcmd *)root)->op_type;
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
			treeprint(((t_opcmd *)root)->left, level + 1);
			treeprint(((t_opcmd *)root)->right, level + 1);
		}
		if (root->type == SUBSHELL)
		{
			printf("SUBSHELL\n");
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
		if (root->type == CMD)
		{
			printf("CMD\n");
			/* printf("-COMMAND\n");
			if (((t_execcmd *)root)->args)
			{
				write(1, ((t_argcmd *)((t_execcmd *)root)->args->content)->s_arg, \
				((t_argcmd *)((t_execcmd *)root)->args->content)->e_arg - \
				((t_argcmd *)((t_execcmd *)root)->args->content)->s_arg);
				printf("\n");
				((t_execcmd *)root)->args = ((t_execcmd *)root)->args->next;
			} */
			/* printf("-SUFFIX\n");
			printf("--ARGS\n");
			while (((t_execcmd *)root)->args)
			{
				write(1, ((t_argcmd *)((t_execcmd *)root)->args->content)->s_arg, \
				((t_argcmd *)((t_execcmd *)root)->args->content)->e_arg - \
				((t_argcmd *)((t_execcmd *)root)->args->content)->s_arg);
				write(1, " | ", 3);
				((t_execcmd *)root)->args = ((t_execcmd *)root)->args->next;
			}
			printf("\n"); */
			/* printf("--REDIRS\n");
			while (((t_execcmd *)root)->redirs)
			{
				write(1, ((t_redircmd *)((t_execcmd *)root)->redirs->content)->s_spec, \
				((t_redircmd *)((t_execcmd *)root)->redirs->content)->e_spec - \
				((t_redircmd *)((t_execcmd *)root)->redirs->content)->s_spec);
				printf(" - ");
				t_redir rtype = ((t_redircmd *)((t_execcmd *)root)->redirs->content)->redir_type;
				switch (rtype)
				{
					case 1:	
						printf("INPUT - ");
						break;
					case 2:
						printf("OUTPUT - ");
						break;
					case 3:
						printf("APPEND - ");
						break;
					case 4:
						printf("HEREDOC - ");
						break;
					default:
						printf("NONE - ");
						break;
				}
				printf("FD:%i\n", ((t_redircmd *)((t_execcmd *)root)->redirs->content)->fd);
				((t_execcmd *)root)->redirs = ((t_execcmd *)root)->redirs->next;
			} */
		}
}

void	mini_panic(char *str)
{
	ft_putstr_fd(str, STDOUT_FILENO);
	exit(1);
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

	(void)argv;
	(void)argc;
	msh->env = env;
	while (1)
	{
		msh->user = get_user();
		prompt = get_prompt(msh);
		if (!prompt)
			mini_panic("An error occured.");
		line = readline(prompt);
		add_history(line);
		t_cmd *root = parser(line, line + ft_strlen(line));
		if (!root)
			continue ;
		//printf("%p\n", root->right);
		//executor(root, msh);
		treeprint(root, 0);
		free(line);
		free(prompt);
	}
	return (0);
}
