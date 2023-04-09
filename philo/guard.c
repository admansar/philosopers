/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guard.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 01:23:26 by admansar          #+#    #+#             */
/*   Updated: 2023/03/25 01:23:53 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

	if (str->philo->number_of_philosophers <= 0)
		i = printf("all args must be positive strict \n");
	else if (str->philo->time_to_die <= 0)
		i = printf("all args must be positive strict \n");
	else if (str->philo->time_to_eat <= 0)
		i = printf("all args must be positive strict \n");
	else if (str->philo->time_to_sleep <= 0)
		i = printf("all args must be positive strict \n");
	else
		i = 0;
	return (i);
}
