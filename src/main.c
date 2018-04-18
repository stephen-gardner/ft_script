/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:27:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/18 00:16:58 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/uio.h>
#include <unistd.h>
#include "ft_getopt.h"
#include "ft_script.h"

const char	*g_pname;

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
	s->file = (g_optind < ac) ? av[g_optind] : DEFAULT_FILE;
	if (++g_optind < ac)
		s->cmd = &av[g_optind];
	return (TRUE);
}

static void		setup_shell(t_session *s, char *const env[])
{
	int	key_len;

	key_len = LEN(SHELL_KEY);
	while (*env)
	{
		if (!ft_strncmp(*env, SHELL_KEY, key_len) && *(*env + key_len) == '=')
			break ;
		++env;
	}
	s->shell = (*env) ? *env + key_len + 1 : DEFAULT_SHELL;
}

int				main(int ac, char *av[], char **env)
{
	t_session	s;

	g_pname = av[0] + 2;
	ft_memset(&s, 0, sizeof(t_session));
	if (!parse_flags(&s, ac, av))
	{
		script_err("usage: script [-adpqr] [-F pipe] [file [command ...]]");
		return (1);
	}
	setup_shell(&s, env);
	return (0);
}

void			script_err(const char *err)
{
	static char		nl = '\n';
	struct iovec	out[2];

	out[0].iov_base = (void *)err;
	out[0].iov_len = LEN(err);
	out[1].iov_base = &nl;
	out[1].iov_len = 1;
	writev(STDERR_FILENO, out, 2);
}
