/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cop.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 01:26:41 by admansar          #+#    #+#             */
/*   Updated: 2023/03/25 01:26:47 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	we_got_him(t_struct *str, int i, int *a)
{
	pthread_mutex_unlock(str->mutexvalue);
	(*a) = 1;
	pthread_mutex_lock(str->mutexwrite);
	str->breaker = 0;
	if (str->philo->number_of_times_each_philosopher_must_eat)
		printf("%ld %d died\n", c_time(str->start_time), i + 1);
	if (str->philo->number_of_philosophers == 1)
		pthread_mutex_unlock(&str->philo->mutexfork[0]);
	pthread_mutex_unlock(str->mutexwrite);
	pthread_mutex_lock(str->mutexvalue);
}

void	*cop(t_struct *str)
{
	int			a;
	long int	*time_passed_from_last_meal;
	int			i;
	long int	*last_meal;

	i = 0;
	last_meal = str->last_meal;
	time_passed_from_last_meal = malloc(sizeof(long int)
			* str->philo->number_of_philosophers);
	a = 0;
	while (!a)
	{
		pthread_mutex_lock(str->mutextime);
		time_passed_from_last_meal[i] = c_time(str->start_time) - last_meal[i];
		pthread_mutex_unlock(str->mutextime);
		pthread_mutex_lock(str->mutexvalue);
		if (time_passed_from_last_meal[i] >= str->philo->time_to_die
			&& str->breaker == 1)
			we_got_him(str, i, &a);
		pthread_mutex_unlock(str->mutexvalue);
		usleep(1);
		i = (i + 1) % (str->philo->number_of_philosophers);
	}
	free(time_passed_from_last_meal);
	return (NULL);
}
