/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/10/03 17:43:50 by emyildir         ###   ########.fr       */
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
		if (root->type == EXEC)
		{
			printf("EXEC\n");
		}
}

void	mini_panic(char *str, int exit_flag, int exit_status)
{
	ft_putstr_fd("msh: ", STDOUT_FILENO);
	ft_putstr_fd(str, STDOUT_FILENO);
	if (exit_flag)
		exit(exit_status);
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
			mini_panic("Readline error", false, false);
		line = readline(prompt);
		if (!line) 
			exit(0);
		add_history(line);
		t_cmd *root = parser(line, line + ft_strlen(line));
		if (!root)
			continue ;
		//printf("%p\n", root->right);
		executor(root, msh);
		//treeprint(root, 0);
		free(line);
		free(prompt);
	}
	return (0);
}
