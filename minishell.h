/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/08/19 23:12:59 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
	
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include "lib/libft/libft.h"


typedef	struct s_msh
{
	char	*user;
	char	**env;
}	t_msh;

typedef struct	s_cmd
{
	int	type;
}	t_cmd;

char	*get_user(t_msh *msh);
int		str_append(char **s1, char const *s2);


#endif