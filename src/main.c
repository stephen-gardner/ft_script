/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:27:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/21 20:24:13 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt.h"
#include "ft_script.h"

const t_opt	g_opt[] = {
	{ 'a', APPEND, 0 },
	{ 'd', INSTANT, 0 },
	{ 'F', FLUSH, 0 },
	{ 'k', KEYLOG, 0 },
	{ 'p', PLAYBACK, TIMESTAMP },
	{ 'q', QUIET, 0 },
	{ 'r', TIMESTAMP, PLAYBACK }
};
int			g_opt_count = sizeof(g_opt) / sizeof(t_opt);

static t_bool	set_flag(t_session *s, int f)
{
	t_opt	opt;
	int		i;

	i = 0;
	while (i < g_opt_count)
	{
		opt = g_opt[i++];
		if (f == opt.flag)
		{
			s->flags &= ~(opt.conflicts);
			s->flags |= opt.val;
			return (TRUE);
		}
	}
	return (FALSE);
}

static t_bool	parse_flags(t_session *s, int ac, char *const av[])
{
	while (ft_getopt(ac, av, OPTIONS) > -1)
	{
		if (!set_flag(s, g_optopt))
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
