/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:08 by moztop            #+#    #+#             */
/*   Updated: 2024/10/18 11:29:24 by moztop           ###   ########.fr       */
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
	else if (((t_cmd *)cmd)->type == REDIR)
		ft_lstclear(&((t_redircmd *)cmd)->args, free);
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

int	lst_add_back_content(t_list **lst, void *content)
{
	t_list	*item;

	item = ft_lstnew(content);
	if (!item || !lst)
		return (free(item), 0);
	ft_lstadd_back(lst, item);
	return (1);
}
