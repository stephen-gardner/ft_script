/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:48:31 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/18 00:16:50 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCRIPT_H
# define FT_SCRIPT_H
# include <sys/time.h>
# include "libft.h"

# define OPTIONS		"adFpqr"
# define DEFAULT_FILE	"typescript"
# define DEFAULT_SHELL	"/bin/sh"
# define SHELL_KEY		"SHELL"

/*
** -a	O_APPEND
** -d	Ignore timing information in typescript
** -F	Flushes immediately after writing, without timing information
** -p	Plays back typescript
** -q	Quiet mode; no status messages
** -r	Record
*/

enum			e_htype
{
	INPUT = 'i',
	END = 'e',
	OUTPUT = 'o',
	START = 's'
};

typedef struct	s_header
{
	size_t		size;
	time_t		tv_sec;
	suseconds_t	tv_usec;
	int			type;
}				t_header;

/*
** script [-adpqr] [-F pipe] [file [command ...]]
*/

typedef struct	s_session
{
	char *const	*cmd;
	char		*shell;
	char		*file;
	int			append: 1;
	int			flush: 1;
	int			instant: 1;
	int			quiet: 1;
	int			record: 1;
}				t_session;

/*
** main.c
*/

void			script_err(const char *err);

extern const char	*g_pname;
#endif
