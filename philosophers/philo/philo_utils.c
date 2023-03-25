/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 01:12:37 by admansar          #+#    #+#             */
/*   Updated: 2023/03/25 01:17:49 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	count;
	int	d;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	d = 0;
	count = 1;
	if (str[i] == '-')
	{
		count = -1;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57 && str[i])
	{
		d = d * 10;
		d = d + str[i] - 48;
		i++;
	}
	return (count * d);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	printer(long int current_time, long int is, t_struct *str, char *msg)
{
	pthread_mutex_lock(str->mutexwrite);
	if (str->breaker == 1)
		printf("%ld %ld %s\n", current_time, is, msg);
	pthread_mutex_unlock(str->mutexwrite);
}
