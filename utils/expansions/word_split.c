#include "../../minishell.h"

int	split_words(t_list *newargs, t_list *explst, char *arg)
{
	t_write *const	wrt = &(t_write){0};
	t_list	*explstword;
	char	*expanded;
	int		status;

	expanded = get_nextarg(wrt, explst, explstword, arg);
	while (expanded)
	{
		status = expand_wildcard(&newargs, explstword, expanded);
		if (!status && !lst_addback_content(&newargs,
				unquote_arg(explstword, expanded)))
			return (ft_lstclear(&newargs, free), 0);
		else if (status == -1)
			return (ft_lstclear(&newargs, free), 0);
		free(expanded);
		ft_lstclear(&explstword, free);
		explstword = NULL;
		expanded = get_nextarg(wrt, explst, explstword, arg);
	}
	return (free(arg), 1);
}

char	*get_nextarg(t_write *wrt, t_list *explst, t_list *explstword, char *arg)
{
	char	*nextarg;
	int		start;

	while (arg[wrt->a_i] && str_include(SPACES, arg[wrt->a_i]))
		wrt->a_i++;
	start = wrt->a_i;
	while (arg[wrt->a_i] && !str_include(SPACES, arg[wrt->a_i]))
		wrt->a_i++;
	return (NULL);
}
