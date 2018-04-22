/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/21 00:52:39 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/21 20:33:09 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include "ft_script.h"

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
	t_timeval		tstamp;
	t_header		header;
	struct iovec	out[2];
	int				i;

	gettimeofday(&tstamp, NULL);
	header.size = len;
	header.tv_sec = tstamp.tv_sec;
	header.tv_usec = tstamp.tv_usec;
	header.type = type;
	write((type == INPUT) ? s->master : STDOUT_FILENO, str, len);
	i = 0;
	if (FL(TIMESTAMP))
	{
		out[i].iov_base = &header;
		out[i++].iov_len = sizeof(t_header);
	}
	if (type != INPUT || FL(TIMESTAMP | KEYLOG))
	{
		out[i].iov_base = str;
		out[i++].iov_len = len;
	}
	writev(s->fd, out, i);
}

static void			notice(t_session *s, int status)
{
	struct iovec	out[6];
	int				i;

	i = 0;
	if (!status)
	{
		out[i].iov_base = "\n";
		out[i++].iov_len = 1;
	}
	out[i].iov_base = "Script ";
	out[i++].iov_len = 7;
	out[i].iov_base = (status) ? "started" : "done";
	out[i++].iov_len = (status) ? 7 : 4;
	out[i].iov_base = ", output file is ";
	out[i++].iov_len = 17;
	out[i].iov_base = s->file;
	out[i++].iov_len = LEN(s->file);
	out[i].iov_base = "\n";
	out[i++].iov_len = 1;
	writev(STDOUT_FILENO, out, i);
}

static void			record_loop(t_session *s)
{
	char	buf[4096];
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
			if ((bytes = read(STDIN_FILENO, buf, 4096)) <= 0)
				break ;
			record(s, buf, bytes, INPUT);
		}
		if (FD_ISSET(s->master, &fds))
		{
			if ((bytes = read(s->master, buf, 4096)) <= 0)
				break ;
			record(s, buf, bytes, OUTPUT);
		}
	}
	record(s, NULL, 0, END);
}

void				record_session(t_session *s)
{
	const char	*path;

	if (!FL(QUIET))
		notice(s, 1);
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
		return ((void)script_err(PNAME, s->av[0], ERRMSG));
	}
	close(s->master);
	if (!FL(QUIET))
		notice(s, 0);
}
