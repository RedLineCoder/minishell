/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/08/19 22:05:06 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
	
# include <readline/readline.h>
# include <readline/history.h>
# include "lib/libft/libft.h"
# include <limits.h>


typedef	struct s_msh
{
	char	*user;
	char	**env;
}	t_msh;

typedef struct	s_cmd
{
	int	type;
}	t_cmd;


#endif