/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/14 18:29:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/19 17:07:36 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrev(char *str)
{
	size_t	len;
	size_t	half;
	size_t	i;
	char	swap;

	len = ft_strlen(str);
	half = len / 2;
	i = 0;
	while (i < half)
	{
		swap = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = swap;
		++i;
	}
	return (str);
}
