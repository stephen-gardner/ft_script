/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 17:35:12 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/22 05:44:16 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/uio.h>
#include <unistd.h>
#include "ft_script.h"

char	*get_env(char *const *env, const char *key)
{
	int	len;

	len = LEN(key);
	while (*env)
	{
		if (!ft_strncmp(*env, key, len) && *(*env + len) == '=')
			return (*env + len + 1);
		++env;
	}
	return (NULL);
}

void	notice(t_session *s, int status)
{
	t_iovec	out[6];
	int		i;

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

int		script_err(const char *pre, const char *err, const char *arg)
{
	t_iovec	out[6];
	int		i;

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

void	write_timestamp(t_session *s, time_t uet, int status)
{
	t_iovec	out[5];
	char	*tstamp;
	int		i;

	tstamp = ctime(&uet);
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
	out[i].iov_base = " on ";
	out[i++].iov_len = 4;
	out[i].iov_base = tstamp;
	out[i++].iov_len = LEN(tstamp);
	writev((FL(PLAYBACK)) ? STDOUT_FILENO : s->fd, out, i);
}
