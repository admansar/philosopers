/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 12:28:45 by admansar          #+#    #+#             */
/*   Updated: 2023/03/25 01:29:03 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating_left_hand(t_struct *str, int is, int *eat_times)
{
	int	left;
	int	right;

	right = is;
	left = is + 1;
	if (left == str->philo->number_of_philosophers)
		left = 0;
	pthread_mutex_lock(&str->philo->mutexfork[left]);
	printer(c_time(str->start_time), is + 1, str, "has taken a fork");
	pthread_mutex_lock(&str->philo->mutexfork[right]);
	printer(c_time(str->start_time), is + 1, str, "has taken a fork");
	printer(c_time(str->start_time), is + 1, str, "is eating");
	pthread_mutex_lock(str->mutextime);
	str->last_meal[is] = c_time(str->start_time);
	pthread_mutex_unlock(str->mutextime);
	(*eat_times)--;
	msleep(str->philo->time_to_eat, str);
	pthread_mutex_unlock(&str->philo->mutexfork[left]);
	pthread_mutex_unlock(&str->philo->mutexfork[right]);
}

void	eating_right_hand(t_struct *str, int is, int *eat_times)
{
	int	left;
	int	right;

	right = is;
	left = is + 1;
	if (left == str->philo->number_of_philosophers)
		left = 0;
	usleep(100);
	pthread_mutex_lock(&str->philo->mutexfork[right]);
	printer(c_time(str->start_time), is + 1, str, "has taken a fork");
	pthread_mutex_lock(&str->philo->mutexfork[left]);
	printer(c_time(str->start_time), is + 1, str, "has taken a fork");
	printer(c_time(str->start_time), is + 1, str, "is eating");
	pthread_mutex_lock(str->mutextime);
	str->last_meal[is] = c_time(str->start_time);
	pthread_mutex_unlock(str->mutextime);
	(*eat_times)--;
	msleep(str->philo->time_to_eat, str);
	pthread_mutex_unlock(&str->philo->mutexfork[right]);
	pthread_mutex_unlock(&str->philo->mutexfork[left]);
}

void	*philo_basic_life(void *philo)
{
	int			is;
	int			eat_times;
	t_struct	*str;

	str = (t_struct *)philo;
	pthread_mutex_lock(str->mutexvalue);
	is = *((t_struct *)philo)->index;
	eat_times = str->philo->number_of_times_each_philosopher_must_eat;
	pthread_mutex_unlock(str->mutexvalue);
	pthread_mutex_lock(str->mutexvalue);
	while (str->breaker && eat_times)
	{
		printer(c_time(str->start_time), is + 1, str, "is thinking");
		pthread_mutex_unlock(str->mutexvalue);
		if (is % 2 == 1)
			eating_left_hand(str, is, &eat_times);
		else
			eating_right_hand(str, is, &eat_times);
		printer(c_time(str->start_time), is + 1, str, "is sleeping");
		msleep(str->philo->time_to_sleep, str);
		pthread_mutex_lock(str->mutexvalue);
	}
	str->philo->number_of_times_each_philosopher_must_eat = eat_times;
	pthread_mutex_unlock(str->mutexvalue);
	return (NULL);
}
