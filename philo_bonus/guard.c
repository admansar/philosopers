/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guard.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 01:23:26 by admansar          #+#    #+#             */
/*   Updated: 2023/04/09 01:37:06 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	guard(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (1)
	{
		j = 0;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
			{
				printf("wrong args in :\\\n");
				return (1);
			}
			j++;
		}
		i++;
		if (i == ac)
			break ;
	}
	return (0);
}

int	bad_num(t_struct *str)
{
	int	i;

	if (str->number_of_philosophers <= 0)
		i = printf("all args must be positive strict \n");
	else if (str->time_to_die <= 0)
		i = printf("all args must be positive strict \n");
	else if (str->time_to_eat <= 0)
		i = printf("all args must be positive strict \n");
	else if (str->time_to_sleep <= 0)
		i = printf("all args must be positive strict \n");
	else
		i = 0;
	return (i);
}
