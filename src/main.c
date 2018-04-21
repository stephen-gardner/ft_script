/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:27:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/20 22:48:19 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt.h"
#include "ft_script.h"

static t_bool	parse_flags(t_session *s, int ac, char *const av[])
{
	char				f;

	while ((f = ft_getopt(ac, av, OPTIONS)) > -1)
	{
		if (f == 'a')
			s->append = TRUE;
		else if (f == 'd')
			s->instant = TRUE;
		else if (f == 'F')
			s->flush = TRUE;
		else if (f == 'p')
			s->record = FALSE;
		else if (f == 'q')
			s->quiet = TRUE;
		else if (f == 'r')
			s->record = TRUE;
		else
			return (FALSE);
	}
	s->file = (g_optind < ac) ? av[g_optind++] : DEFAULT_FILE;
	return (TRUE);
}

static void		setup_env(t_session *s, int ac, char *av[], char *const env[])
{
	if (g_optind < ac)
		ft_memmove(&av[0], &av[g_optind], sizeof(char *) * (ac - g_optind + 1));
	else
	{
		if (!(av[0] = get_env(env, SHELL_KEY)))
			av[0] = DEFAULT_SHELL;
		av[1] = NULL;
	}
	s->av = av;
	s->env = env;
}

int				main(int ac, char *av[], char *env[])
{
	t_session	s;

	ft_memset(&s, 0, sizeof(t_session));
	if (!parse_flags(&s, ac, av))
	{
		script_err("usage", PNAME " [-adpqr] [-F pipe] [file [command ...]]",
			NULL);
		return (1);
	}
	setup_env(&s, ac, av, env);
	start_session(&s);
	return (0);
}
