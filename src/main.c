/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:27:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/19 00:31:32 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/uio.h>
#include <unistd.h>
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
	s->file = (g_optind < ac) ? av[g_optind] : DEFAULT_FILE;
	if (++g_optind < ac)
		s->cmd = &av[g_optind];
	return (TRUE);
}

static void		setup_shell(t_session *s, char *const env[])
{
	int	key_len;

	s->env = env;
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

	ft_memset(&s, 0, sizeof(t_session));
	if (!parse_flags(&s, ac, av))
	{
		script_err("usage", PNAME " [-adpqr] [-F pipe] [file [command ...]]",
			NULL);
		return (1);
	}
	setup_shell(&s, env);
	start_session(&s);
	return (0);
}

int				script_err(const char *pre, const char *err, const char *arg)
{
	struct iovec	out[6];
	int				i;

	i = 0;
	if (pre)
	{
		out[i].iov_base = (void *)pre;
		out[i++].iov_len = (pre) ? LEN(pre) : 0;
		out[i].iov_base = ": ";
		out[i++].iov_len = 2;
	}
	out[i].iov_base = (void *)err;
	out[i++].iov_len = LEN(err);
	if (arg)
	{
		out[i].iov_base = ": ";
		out[i++].iov_len = 2;
		out[i].iov_base = (void *)arg;
		out[i++].iov_len = LEN(arg);
	}
	out[i].iov_base = "\n";
	out[i++].iov_len = 1;
	writev(STDERR_FILENO, out, i);
	return (-1);
}
