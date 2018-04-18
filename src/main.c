/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:27:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/17 19:10:50 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt.h"
#include "ft_script.h"

int	main(int ac, char **av, char **env)
{
	t_session	s;

	ft_memset(&s, 0, sizeof(t_session));
	parse_flags(&s, ac, av);
	return (0);
}
