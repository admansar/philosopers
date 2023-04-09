/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guard.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 01:19:28 by admansar          #+#    #+#             */
/*   Updated: 2023/03/25 01:19:32 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_all_mutex(t_struct *str, int *i)
{
	while ((*i)--)
		if (pthread_mutex_init(&str->philo->mutexfork[(*i)], NULL))
			return (0);
	if (pthread_mutex_init(str->mutexwrite, NULL))
		return (0);
	if (pthread_mutex_init(str->mutexvalue, NULL))
		return (0);
	if (pthread_mutex_init(str->mutextime, NULL))
		return (0);
	return (1);
}

int	creat_threads(t_struct *str, int *i)
{
	while (++(*i) < str->philo->number_of_philosophers)
	{
		pthread_mutex_unlock(str->mutexvalue);
		str->index = i;
		str->last_meal[(*i)] = 0;
		if (pthread_create(&str->thread[(*i)], NULL, philo_basic_life,
				(void *)str))
			return (0);
		if ((*i) > 100)
			usleep((*i));
		else
			usleep(80);
		pthread_mutex_lock(str->mutexvalue);
	}
	pthread_mutex_unlock(str->mutexvalue);
	return (1);
}

int	join_threads(t_struct *str, int *i)
{
	(*i) = str->philo->number_of_philosophers;
	while ((*i)--)
		if (pthread_join(str->thread[(*i)], NULL))
			return (0);
	return (1);
}

int	destroy_threads(t_struct *str, int *i)
{
	(*i) = str->philo->number_of_philosophers;
	while ((*i)--)
		if (pthread_mutex_destroy(&str->philo->mutexfork[(*i)]))
			return (0);
	if (pthread_mutex_destroy(str->mutexwrite))
		return (0);
	if (pthread_mutex_destroy(str->mutexvalue))
		return (0);
	if (pthread_mutex_destroy(str->mutextime))
		return (0);
	return (1);
}

int	creat_join_destroy_theads(t_struct *str, int i)
{
	str->start_time = c_time(0);
	pthread_mutex_lock(str->mutexvalue);
	if (!creat_threads(str, &i))
		return (0);
	cop(str);
	if (!join_threads(str, &i))
		return (0);
	if (!destroy_threads(str, &i))
		return (0);
	return (1);
}
