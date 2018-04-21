/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 00:20:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/21 00:35:07 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ft_script.h"
/*
static t_bool	open_log(t_session *s)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (s->append)
		flags |= O_APPEND;
	if ((s->fd = open(s->file, flags, 0644)) < 0)
	{
		script_err(PNAME, s->file, ERRMSG);
		return (FALSE);
	}
	return (TRUE);
}*/

static const char		*find_app(t_session *s)
{
	struct stat	fstat;
	const char	*app;
	char		*path;
	char		*delim;

	path = get_env(s->env, "PATH");
	while (path && *path)
	{
		if ((delim = ft_strchr(path, ':')))
			*delim = '\0';
		if ((app = build_path(path, s->av[0]))
			&& !access(app, X_OK)
			&& !stat(app, &fstat)
			&& (fstat.st_mode & S_IFMT) != S_IFDIR)
		{
			if (delim)
				*delim = ':';
			s->av[0] = (char *)app;
			return (app);
		}
		if (delim)
			*delim = ':';
		path = (delim) ? delim + 1 : NULL;
	}
	return (NULL);
}

void			start_session(t_session *s)
{
	char		buf[4096];
	const char	*path;
	int			master;
	int			bytes;
	fd_set		fds;

	if (ft_forkpty(&master, NULL, get_winsize()))
	{
		toggle_raw(TRUE);
		while (TRUE)
		{
			FD_ZERO(&fds);
			FD_SET(STDIN_FILENO, &fds);
			FD_SET(master, &fds);
			select(master + 1, &fds, NULL, NULL, NULL);
			if (FD_ISSET(STDIN_FILENO, &fds))
			{
				if ((bytes = read(STDIN_FILENO, buf, 4096)) <= 0)
					break ;
				write(master, buf, bytes);
			}
			if (FD_ISSET(master, &fds))
			{
				if ((bytes = read(master, buf, 4096)) <= 0)
					break ;
				write(STDOUT_FILENO, buf, bytes);
			}
		}
		close(master);
		toggle_raw(FALSE);
	}
	else
	{
		path = s->av[0];
		if (*path != '/' && ft_strncmp(path, "./", 2))
			path = find_app(s);
		if (path)
			execve(path, s->av, s->env);
		script_err(PNAME, s->av[0], ERRMSG);
	}
}
