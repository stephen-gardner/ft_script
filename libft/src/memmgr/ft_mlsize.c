/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mllen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 00:52:38 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/12 00:57:05 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memmgr.h"

size_t	ft_mlsize(t_mlink *mlink)
{
	size_t	len;

	len = 0;
	while (mlink)
	{
		len += mlink->size;
		mlink = mlink->next;
	}
	return (len);
}
