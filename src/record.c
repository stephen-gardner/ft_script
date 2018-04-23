/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/21 00:52:39 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/23 04:49:36 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include "ft_script.h"

static const char	*build_path(const char *path, const char *app)
{
	static char	res[PATH_MAX + 1];
	int			path_len;
	int			app_len;

	path_len = LEN(path);
	app_len = LEN(app);
	if (path_len + app_len + (path[path_len - 1] != '/') > PATH_MAX)
		return (NULL);
	ft_memcpy(res, path, path_len);
	if (res[path_len - 1] != '/')
		res[path_len++] = '/';
	ft_memcpy(res + path_len, app, app_len);
	res[path_len + app_len] = '\0';
	return (res);
}

static const char	*find_app(t_session *s)
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

static void			record(t_session *s, char *str, int len, char type)
{
	t_timeval	tstamp;
	t_header	header;
	t_iovec		out[2];
	int			i;

	gettimeofday(&tstamp, NULL);
	header.size = len;
	header.tv_sec = tstamp.tv_sec;
	header.tv_usec = tstamp.tv_usec;
	header.type = type;
	write((type == INPUT) ? s->master : STDOUT_FILENO, str, len);
	i = 0;
	if (FL(TIMED))
	{
		out[i].iov_base = &header;
		out[i++].iov_len = sizeof(t_header);
	}
	if (type != INPUT || FL(KEYLOG | TIMED))
	{
		out[i].iov_base = str;
		out[i++].iov_len = len;
	}
	writev(s->fd, out, i);
}

static void			record_loop(t_session *s)
{
	char	buf[BUFF_SIZE];
	fd_set	fds;
	int		bytes;

	record(s, NULL, 0, START);
	while (TRUE)
	{
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		FD_SET(s->master, &fds);
		select(s->master + 1, &fds, NULL, NULL, NULL);
		if (FD_ISSET(STDIN_FILENO, &fds))
		{
			if ((bytes = read(STDIN_FILENO, buf, BUFF_SIZE)) <= 0)
				break ;
			record(s, buf, bytes, INPUT);
		}
		if (FD_ISSET(s->master, &fds))
		{
			if ((bytes = read(s->master, buf, BUFF_SIZE)) <= 0)
				break ;
			record(s, buf, bytes, OUTPUT);
		}
	}
	record(s, NULL, 0, END);
}

int					record_session(t_session *s)
{
	const char	*path;

	if (ft_forkpty(&s->master, NULL, get_winsize()))
	{
		term_setraw(1);
		record_loop(s);
		term_setraw(0);
	}
	else
	{
		close(s->fd);
		path = s->av[0];
		if (*path != '/' && ft_strncmp(path, "./", 2))
			path = find_app(s);
		if (path)
			execve(path, s->av, s->env);
		return (script_err(PNAME, s->av[0], ERRMSG));
	}
	close(s->master);
	return (0);
}
