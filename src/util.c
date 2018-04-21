/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 17:35:12 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/21 06:36:20 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <sys/uio.h>
#include <unistd.h>
#include "ft_script.h"

const char	*build_path(const char *path, const char *app)
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

char		*get_env(char *const *env, const char *key)
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

int			script_err(const char *pre, const char *err, const char *arg)
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
