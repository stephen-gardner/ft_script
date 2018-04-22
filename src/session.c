/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 00:20:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/22 00:30:23 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "ft_script.h"

static t_bool	open_log(t_session *s)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	flags |= ((FL(APPEND)) ? O_APPEND : O_TRUNC);
	if ((s->fd = open(s->file, flags, 0644)) < 0)
	{
		script_err(PNAME, s->file, ERRMSG);
		return (FALSE);
	}
	return (TRUE);
}

void			start_session(t_session *s)
{
	open_log(s);
	if (FL(PLAYBACK))
		;
	else
	{
		if (!FL(QUIET))
			notice(s, 1);
		record_session(s);
		if (!FL(QUIET))
			notice(s, 0);
	}
	close(s->fd);
}
