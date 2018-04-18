/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/03 16:20:06 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/16 16:51:12 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_getopt.h"
#include "libft.h"

char		*g_optarg;
int			g_optind = 1;
int			g_opterr = 1;
int			g_optopt;

static int		opt_error(char *pname, const char *err, const char *optstring)
{
	char	tmp[6];

	if (g_opterr && *optstring != ':')
	{
		write(STDERR_FILENO, pname, LEN(pname));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, err, LEN(err));
		ft_stpcpy(tmp, " -- ");
		tmp[4] = g_optopt;
		tmp[5] = '\n';
		write(STDERR_FILENO, tmp, 6);
	}
	return ((*optstring == ':') ? ':' : '?');
}

static t_bool	set_arg(char *const av[], char **pos, const char *op)
{
	t_bool	optional;

	g_optarg = NULL;
	if (*(op + 1) != ':')
		return (TRUE);
	g_optarg = (**pos) ? *pos : av[g_optind];
	optional = (*op && *(op + 2) == ':');
	if (optional && g_optarg && *g_optarg == '-')
		g_optarg = NULL;
	else
		++g_optind;
	*pos = NULL;
	return (g_optarg != NULL || optional);
}

static int		process_arg(char *const av[], const char *optstring)
{
	static char	*pos;
	const char	*op;

	if (!pos || !*pos)
		pos = av[g_optind] + 1;
	g_optopt = *pos++;
	if (!*pos)
		++g_optind;
	if (!(op = ft_strchr(optstring, g_optopt)))
		return (opt_error(av[0], "illegal option", optstring));
	if (!set_arg(av, &pos, op))
		return (opt_error(av[0], "option requires an argument", optstring));
	return (g_optopt);
}

int				ft_getopt(int ac, char *const av[], const char *optstring)
{
	if (g_optind == 1)
	{
		g_optarg = NULL;
		g_optopt = '\0';
	}
	if (g_optind < ac)
	{
		if (*av[g_optind] != '-' || !*(av[g_optind] + 1))
		{
			if (*optstring != '-')
				return (-1);
			g_optarg = av[g_optind++];
			return ((g_optopt = '\1'));
		}
		if (!ft_strcmp(av[g_optind], "--"))
		{
			g_optopt = '-';
			++g_optind;
			return (-1);
		}
		return (process_arg(av, optstring));
	}
	return (-1);
}
