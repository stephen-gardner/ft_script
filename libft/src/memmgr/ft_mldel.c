/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mldel.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 00:54:52 by sgardner          #+#    #+#             */
/*   Updated: 2017/12/03 02:04:24 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_memmgr.h"

t_mlink		*ft_mldel(t_mlink *mlink)
{
	free(mlink->ptr);
	return (ft_mlremove(mlink));
}
