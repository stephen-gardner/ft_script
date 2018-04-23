/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playback.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/21 00:53:07 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/22 22:15:02 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_script.h"

static void		sleep_diff(t_timespec *prev, t_timespec *curr)
{
	t_timespec	diff;

	if ((diff.tv_sec = curr->tv_sec - prev->tv_sec) < 0)
		diff.tv_sec = 0;
	if ((diff.tv_nsec = curr->tv_nsec - prev->tv_nsec) < 0)
	{
		if (diff.tv_sec)
		{
			--diff.tv_sec;
			diff.tv_nsec += 100000000;
		}
		else
			diff.tv_nsec = 0;
	}
	nanosleep(&diff, NULL);
}

static t_bool	process_log(t_session *s, t_header *h, char *data)
{
	static t_timespec	prev;
	static t_timespec	curr;

	curr.tv_sec = h->tv_sec;
	curr.tv_nsec = h->tv_usec * 1000;
	if (h->type == OUTPUT)
	{
		if (!FL(INSTANT))
			sleep_diff(&prev, &curr);
		write(STDOUT_FILENO, data, h->size);
		prev = curr;
	}
	else if (h->type == INPUT)
		;
	else if (h->type == START)
	{
		write_timestamp(s, h->tv_sec, 1);
		prev = curr;
	}
	else if (h->type == END)
		write_timestamp(s, h->tv_sec, 0);
	else
		return (FALSE);
	return (TRUE);
}

void			playback_session(t_session *s)
{
	char		buf[BUFF_SIZE];
	t_header	curr;
	int			bytes;

	while (TRUE)
	{
		if ((bytes = read(s->fd, &curr, sizeof(t_header))) < 0)
			return ((void)script_err(PNAME, ERRMSG, NULL));
		if (!bytes)
			break ;
		if (bytes < (int)sizeof(t_header)
			|| (size_t)read(s->fd, buf, curr.size) < curr.size
			|| !process_log(s, &curr, buf))
			return ((void)script_err(PNAME, "invalid header", NULL));
	}
}
