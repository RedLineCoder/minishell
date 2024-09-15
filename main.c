/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/09/15 06:02:51 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void treeprint(t_node *root, int level)
{
        if (root == NULL)
			return;
        for (int i = 0; i < level; i++)
                printf(i == level - 1 ? "|-" : "  ");
		if (root->cmd)
		{
			t_tokens token = root->cmd->type;
			switch (token)
			{
				case 1: 	
					printf(" BLOCK\n");
					break;
				case 3:
					printf(" CMD OP\n");
					break;
				case 5:
					printf(" EXEC\n");
					break;
				default:
					printf(" null\n");
					break;
			}
		}
		else
			printf("ROOT\n");
        treeprint(root->left, level + 1);
        treeprint(root->right, level + 1);
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
		t_node *root = get_node(NULL, 0);
		if (!parser(line, root))
			continue ;
		//printf("%p\n", root->right);
		//executor(root, msh);
		treeprint(root, 0);
		clean_tree(root);
		free(line);
		free(prompt);
	}
	return (0);
}
