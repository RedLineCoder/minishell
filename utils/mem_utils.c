/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:08 by moztop            #+#    #+#             */
/*   Updated: 2024/10/04 17:43:27 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_tree(void *cmd)
{
	if (!cmd)
		return ;
	else if (((t_cmd *)cmd)->type == LOGIC)
	{
		clean_tree(((t_logiccmd *)cmd)->left);
		clean_tree(((t_logiccmd *)cmd)->right);
	}
	else if (((t_cmd *)cmd)->type == PIPE)
		ft_lstclear(&((t_pipecmd *)cmd)->pipelist, clean_tree);
	else if (((t_cmd *)cmd)->type == SUBSHELL)
	{
		clean_tree(((t_blockcmd *)cmd)->subshell);
		ft_lstclear(&((t_blockcmd *)cmd)->redirs, clean_tree);
	}
	else if (((t_cmd *)cmd)->type == EXEC)
	{
		ft_lstclear(&((t_execcmd *)cmd)->redirs, clean_tree);
		ft_lstclear(&((t_execcmd *)cmd)->args, free);
	}
	free(cmd);
}

t_part	ft_divide(char *s, char *e, t_tokens tkn, int rev)
{
	t_part	ln;
	char	*ts;
	char	*te;

	ln.lfts = s;
	while (s && s != e)
	{
		pass_block(s, &s, e);
		if (get_token(&s, &e, &ts, &te) == tkn)
		{
			ln.lfte = ts;
			ln.rghts = te;
			if (!rev)
				break ;
		}
	}
	ln.rghte = e;
	return (ln);
}
